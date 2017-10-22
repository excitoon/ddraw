#pragma once

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include <windows.h>

#include "Constants.h"
#include "Logger.h"
#include "Scheduler.h"


class Buffer
{
    inline unsigned short packHiColor(unsigned color)
    {
        unsigned char r = (color >> 16) & 0x1F;
        unsigned char g = (color >> 8) & 0x3F;
        unsigned char b = color & 0x1F;
        return (r << (5+6)) | (g << 5) | b;
    }

    inline unsigned unpackHiColor(unsigned short color)
    {
        unsigned char r = (color & 0xF800) >> (5+6-3);
        unsigned char g = (color & 0x07E0) >> (6-3);
        unsigned char b = (color & 0x001F) << (3);
        return (r << 16) | (g << 8) | b;
    }

    Scheduler * scheduler = nullptr;
    std::condition_variable cv;
    std::mutex mutex;
    volatile bool pending_render = false;
    volatile bool shutting_down = false;
    std::thread background_render;
    IDirectDrawSurface7 * surface;
    std::once_flag background_thread_initialized;

    Logger log = Logger(Logger::Level::Trace, "DirectDrawSurface7::Buffer");

public:
    struct Key
    {
        bool is_null;
        RECT rect;

        struct Hasher
        {
            std::size_t operator ()(const Key & k) const
            {
                if (k.is_null)
                {
                    return 0;
                }
                return (std::hash<LONG>()(k.rect.top) << 3) ^ (std::hash<LONG>()(k.rect.left) << 2)
                    ^ (std::hash<LONG>()(k.rect.bottom) << 1) ^ std::hash<LONG>()(k.rect.right);
            }
        };

        Key(LPRECT lpRect)
        {
            if (lpRect == nullptr)
            {
                is_null = true;
            }
            else
            {
                is_null = false;
                rect = *lpRect;
            }
        }

        bool operator ==(const Key & rhs) const
        {
            if (is_null || rhs.is_null)
            {
                return is_null == rhs.is_null;
            }
            return std::tie(rect.top, rect.left, rect.bottom, rect.right)
                == std::tie(rhs.rect.top, rhs.rect.left, rhs.rect.bottom, rhs.rect.right);
        }
    };

    void * underlying = nullptr;
    std::vector<unsigned char> data;
    DDSURFACEDESC2 DDSurfaceDesc;
    unsigned lock_flags;
    unsigned width;
    unsigned client_width;
    unsigned height;
    unsigned client_height;

    Buffer()
    {
    }

    Buffer(Scheduler * scheduler, IDirectDrawSurface7 * surface, DDSURFACEDESC2 * lpDDSurfaceDesc, unsigned lock_flags):
        scheduler(scheduler),
        surface(surface),
        underlying(lpDDSurfaceDesc->lpSurface),
        DDSurfaceDesc(*lpDDSurfaceDesc),
        lock_flags(lock_flags),
        width(lpDDSurfaceDesc->dwWidth),
        client_width(std::min(width, Constants::MaxPrimarySurfaceBufferWidth)),
        height(lpDDSurfaceDesc->dwHeight),
        client_height(std::min(height, Constants::MaxPrimarySurfaceBufferHeight))
    {
        data.resize(client_width * client_height * 2);
        if (Constants::InitializeBuffersWithZeros)
        {
            this->underlying = underlying;
            std::fill(data.begin(), data.end(), 0);
        }
        else
        {
            load(underlying);
        }
    }

    ~Buffer()
    {
        shutting_down = true;
        cv.notify_one(); /// FIXME. That must be done under lock.
    }

    void load(void * underlying)
    {
        this->underlying = underlying;
        for (unsigned y = 0; y < client_height; ++y)
        {
            unsigned * input = reinterpret_cast<unsigned *>(reinterpret_cast<unsigned char *>(underlying) + y * width * 4);
            unsigned short * output = reinterpret_cast<unsigned short *>(data.data() + y * client_width * 2);
            for (unsigned x = 0; x < client_width; ++x)
            {
                /// TODO. Use SSE.
                *output++ = packHiColor(*input++);
            }
        }
    }

    void render()
    {
        for (unsigned y = 0; y < client_height; ++y)
        {
            unsigned short * input = reinterpret_cast<unsigned short *>(data.data() + y * client_width * 2);
            unsigned * output = reinterpret_cast<unsigned *>(reinterpret_cast<unsigned char *>(underlying) + y * width * 4);
            for (unsigned x = 0; x < client_width; ++x)
            {
                /// TODO. Use SSE, but only in foreground processing because we gonna outrun client application
                /// and catch rendering glitches.
                *output++ = unpackHiColor(*input++);
            }
        }
    }

    void fillSurfaceDescription(DDSURFACEDESC2 & description)
    {
        description.lpSurface = data.data();
        description.lPitch = client_width * 2;
        description.dwWidth = client_width;
        description.dwHeight = client_height;
        description.ddpfPixelFormat.dwRGBBitCount = 16;
        description.ddpfPixelFormat.dwRBitMask = 0xF800;
        description.ddpfPixelFormat.dwGBitMask = 0x07E0; /// 5:6:5
        description.ddpfPixelFormat.dwBBitMask = 0x001F;
        description.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
    }

    bool lock()
    {
        /// TODO. Assuming Lock() arguments are not changing and DDLOCK_WAIT is set in first call.
        /// If we are here then LPRECT is nullptr.
        HRESULT result = scheduler->makeTask<HRESULT>([&]() { return surface->Lock(nullptr, &DDSurfaceDesc, lock_flags, nullptr); });
        if (result == DDERR_SURFACEBUSY)
        {
            log(Logger::Level::Trace) << "Surface is busy, retrying.";
            renderInBackground();
            return false;
        }
        else if (result != DD_OK)
        {
            /// TODO. Possibly we need to try another arguments.
            log(Logger::Level::Warning) << "Lock() failed, result=" << result << ".";
            return false;
        }
        return true;
    }

    void unlock()
    {
        HRESULT result = scheduler->makeTask<HRESULT>([&]() { return surface->Unlock(nullptr); });
        if (result != DD_OK)
        {
            log(Logger::Level::Error) << "Unlock() failed.";
        }
    }

    /// N.B. Only entire buffer is allowed to render in background.
    void renderInBackground()
    {
        std::call_once(background_thread_initialized, [this]()
        {
            background_render = std::thread([this]()
            {
                while (!shutting_down)
                {
                    std::unique_lock<std::mutex> unique_lock(mutex);
                    /// TODO. Optimize that.
                    cv.wait(unique_lock, [this]() { return shutting_down || pending_render; });
                    if (pending_render)
                    {
                        pending_render = false;
                        if (lock())
                        {
                            render();
                            unlock();
                        }
                    }
                }
            });
        });
        if (pending_render)
        {
            return;
        }
        pending_render = true;
        cv.notify_one(); /// FIXME. That must be done under lock.
    }
};
