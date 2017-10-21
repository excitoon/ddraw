#pragma once

#include <vector>
#include <tuple>
#include <utility>
#include <unordered_map>
#include <chrono>

#include <windows.h>

#include "Logger.h"
#include "Unknown.h"
#include "Constants.h"


template <typename SurfaceHolder>
class DirectDrawSurface7 : public Unknown<DirectDrawSurface7<SurfaceHolder>>
{
    IDirectDrawSurface7 * underlying;
    SurfaceHolder & surface_holder;

    bool is_primary;
    std::chrono::system_clock::time_point last_render_time;

    struct BufferKey
    {
        bool is_null;
        RECT rect;

        BufferKey(LPRECT lpRect)
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

        bool operator ==(const BufferKey & rhs) const
        {
            if (is_null || rhs.is_null)
            {
                return is_null == rhs.is_null;
            }
            return std::tie(rect.top, rect.left, rect.bottom, rect.right)
                == std::tie(rhs.rect.top, rhs.rect.left, rhs.rect.bottom, rhs.rect.right); 
        }
    };

    struct BufferKeyHasher
    {
        std::size_t operator ()(const BufferKey & k) const
        {
            if (k.is_null)
            {
                return 0;
            }
            return (std::hash<LONG>()(k.rect.top) << 3) ^ (std::hash<LONG>()(k.rect.left) << 2)
                ^ (std::hash<LONG>()(k.rect.bottom) << 1) ^ std::hash<LONG>()(k.rect.right);
        }
    };

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

    public:
        void * underlying = nullptr;
        std::vector<unsigned char> data;
        unsigned width;
        unsigned height;

        Buffer()
        {
        }

        Buffer(void * underlying, unsigned width, unsigned height):
            width(width),
            height(height)
        {
            data.resize(height*width*4);
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

        void load(void * underlying)
        {
            this->underlying = underlying;
            for (unsigned y = 0; y < height; ++y)
            {
                unsigned * input = reinterpret_cast<unsigned *>(reinterpret_cast<unsigned char *>(underlying) + y * width * 4);
                unsigned short * output = reinterpret_cast<unsigned short *>(data.data() + y * width * 4);
                for (unsigned x = 0; x < width; ++x)
                {
                    *output++ = packHiColor(*input++);                     
                }
            }
        }

        void render()
        {
            for (unsigned y = 0; y < height; ++y)
            {
                unsigned short * input = reinterpret_cast<unsigned short *>(data.data() + y * width * 4);
                unsigned * output = reinterpret_cast<unsigned *>(reinterpret_cast<unsigned char *>(underlying) + y * width * 4);
                for (unsigned x = 0; x < width; ++x)
                {
                    *output++ = unpackHiColor(*input++);
                }
            }
        }
    };

    std::unordered_map<BufferKey, Buffer, BufferKeyHasher> buffers;

    Logger log = Logger(Logger::Level::Trace, "DirectDrawSurface7");

public:
    DirectDrawSurface7(SurfaceHolder & surface_holder, IDirectDrawSurface7 * underlying, bool is_primary):
        Unknown<DirectDrawSurface7>(underlying),
        underlying(underlying),
        surface_holder(surface_holder),
        is_primary(is_primary),
        last_render_time(std::chrono::system_clock::now())
    {
    }

    ~DirectDrawSurface7()
    {
    }

    SurfaceHolder & getSurfaceHolder()
    {
        return surface_holder;
    }

    virtual __stdcall HRESULT AddAttachedSurface(LPDIRECTDRAWSURFACE7 arg)
    {
        log() << "AddAttachedSurface.";
        return underlying->AddAttachedSurface(surface_holder.getOriginalSurface(arg));
    }

    virtual __stdcall HRESULT AddOverlayDirtyRect(LPRECT arg)
    {
        log() << "AddOverlayDirtyRect.";
        return underlying->AddOverlayDirtyRect(arg);
    }

    virtual __stdcall HRESULT Blt(LPRECT arg1, LPDIRECTDRAWSURFACE7 arg2, LPRECT arg3, DWORD arg4, LPDDBLTFX arg5)
    {
        log() << "Blt.";
        return underlying->Blt(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT BltBatch(LPDDBLTBATCH arg1, DWORD arg2, DWORD arg3)
    {
        log() << "BltBatch.";
        return underlying->BltBatch(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT BltFast(DWORD arg1, DWORD arg2, LPDIRECTDRAWSURFACE7 arg3, LPRECT arg4, DWORD arg5)
    {
        log() << "BltFast.";
        return underlying->BltFast(arg1, arg2, surface_holder.getOriginalSurface(arg3), arg4, arg5);
    }

    virtual __stdcall HRESULT DeleteAttachedSurface(DWORD arg1, LPDIRECTDRAWSURFACE7 arg2)
    {
        log() << "DeleteAttachedSurface.";
        return underlying->DeleteAttachedSurface(arg1, surface_holder.getOriginalSurface(arg2));
    }

    struct WrapperContext
    {
        LPVOID lpContext;
        LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback;
        DWORD dwFlags;
        DirectDrawSurface7<SurfaceHolder> & direct_draw_surface7;
    };

    static __stdcall HRESULT EnumSurfacesCallback7(LPDIRECTDRAWSURFACE7 lpDDSurface, LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext)
    {
        WrapperContext & context = *static_cast<WrapperContext *>(lpContext);
        context.direct_draw_surface7.log() << "EnumSurfacesCallback7.";
        return context.lpEnumSurfacesCallback(context.direct_draw_surface7.getSurfaceHolder().getWrappedSurface(lpDDSurface), lpDDSurfaceDesc, context.lpContext);
    }

    virtual __stdcall HRESULT EnumAttachedSurfaces(LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback)
    {
        log() << "EnumAttachedSurfaces.";
        WrapperContext wrapper_context{ lpContext, lpEnumSurfacesCallback, 0, *this };
        return underlying->EnumAttachedSurfaces(static_cast<LPVOID>(&wrapper_context), EnumSurfacesCallback7);
    }

    virtual __stdcall HRESULT EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpfnCallback)
    {
        log() << "EnumOverlayZOrders.";
        WrapperContext wrapper_context{ lpContext, lpfnCallback, dwFlags, *this };
        return underlying->EnumOverlayZOrders(dwFlags, static_cast<LPVOID>(&wrapper_context), EnumSurfacesCallback7);
    }

    virtual __stdcall HRESULT Flip(LPDIRECTDRAWSURFACE7 arg1, DWORD arg2)
    {
        log() << "Flip.";
        return underlying->Flip(surface_holder.getOriginalSurface(arg1), arg2);
    }

    virtual __stdcall HRESULT GetAttachedSurface(LPDDSCAPS2 lpDDSCaps, LPDIRECTDRAWSURFACE7 FAR * lplpDDAttachedSurface)
    {
        log() << "GetAttachedSurface.";
        LPDIRECTDRAWSURFACE7 lpDDAttachedSurface = nullptr;
        HRESULT result = underlying->GetAttachedSurface(lpDDSCaps, &lpDDAttachedSurface);
        *lplpDDAttachedSurface = surface_holder.getWrappedSurface(lpDDAttachedSurface);
        return result;
    }

    virtual __stdcall HRESULT GetBltStatus(DWORD arg)
    {
        log() << "GetBltStatus.";
        return underlying->GetBltStatus(arg);
    }

    virtual __stdcall HRESULT GetCaps(LPDDSCAPS2 arg)
    {
        log() << "GetCaps.";
        return underlying->GetCaps(arg);
    }

    virtual __stdcall HRESULT GetClipper(LPDIRECTDRAWCLIPPER FAR * arg)
    {
        log() << "GetClipper.";
        return underlying->GetClipper(arg);
    }

    virtual __stdcall HRESULT GetColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        log() << "GetColorKey.";
        return underlying->GetColorKey(arg1, arg2);
    }

    virtual __stdcall HRESULT GetDC(HDC FAR * arg)
    {
        log() << "GetDC.";
        return underlying->GetDC(arg);
    }

    virtual __stdcall HRESULT GetFlipStatus(DWORD arg)
    {
        log() << "GetFlipStatus.";
        return underlying->GetFlipStatus(arg);
    }

    virtual __stdcall HRESULT GetOverlayPosition(LPLONG arg1, LPLONG arg2)
    {
        log() << "GetOverlayPosition.";
        return underlying->GetOverlayPosition(arg1, arg2);
    }

    virtual __stdcall HRESULT GetPalette(LPDIRECTDRAWPALETTE FAR * arg)
    {
        log() << "GetPalette.";
        return underlying->GetPalette(arg);
    }

    virtual __stdcall HRESULT GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
    {
        HRESULT result = underlying->GetPixelFormat(lpDDPixelFormat);
        log() << "GetPixelFormat(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec << ")"
            << " -> flags=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwFlags << std::dec
            << ", bpp=" << lpDDPixelFormat->dwRGBBitCount
            << ", r=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwRBitMask << std::dec
            << ", g=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwGBitMask << std::dec
            << ", b=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwBBitMask << std::dec
            << ", a=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwRGBAlphaBitMask << std::dec << ".";
        if (Constants::Emulate16BitsPerPixel)
        {
            lpDDPixelFormat->dwRGBBitCount = 16;
            lpDDPixelFormat->dwRBitMask = 0xF800;           
            lpDDPixelFormat->dwGBitMask = 0x07E0; /// 5:6:5           
            lpDDPixelFormat->dwBBitMask = 0x001F;            
            lpDDPixelFormat->dwRGBAlphaBitMask = 0;            
        }
        return result;
    }

    virtual __stdcall HRESULT GetSurfaceDesc(LPDDSURFACEDESC2 arg)
    {
        log() << "GetSurfaceDesc.";
        return underlying->GetSurfaceDesc(arg);
    }

    virtual __stdcall HRESULT Initialize(LPDIRECTDRAW arg1, LPDDSURFACEDESC2 arg2)
    {
        log() << "Initialize.";
        return underlying->Initialize(arg1, arg2);
    }

    virtual __stdcall HRESULT IsLost()
    {
        log() << "IsLost.";
        return underlying->IsLost();
    }

    virtual __stdcall HRESULT Lock(LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
    {
        HRESULT result = underlying->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent);
        log(Logger::Level::Trace) << "Lock(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", rect=" << std::hex << std::setfill('0') << std::setw(8) << lpDestRect << std::dec
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << dwFlags << std::dec << ")"
            << " -> width=" << lpDDSurfaceDesc->dwWidth << ", height=" << lpDDSurfaceDesc->dwHeight
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << lpDDSurfaceDesc->dwFlags << std::dec << ".";
        if (result == DD_OK)
        {
            if (Constants::Emulate16BitsPerPixel && is_primary)
            {
                if (lpDDSurfaceDesc->dwFlags & DDSD_WIDTH && lpDDSurfaceDesc->dwFlags & DDSD_HEIGHT)
                {
                    auto it = buffers.find(lpDestRect);
                    if (it != buffers.end())
                    {
                        if (Constants::ReadFromSurfaceMemoryOncePerBuffer)
                        {
                            it->second.underlying = lpDDSurfaceDesc->lpSurface;
                        }
                        else
                        {
                            it->second.load(lpDDSurfaceDesc->lpSurface);
                        }
                        lpDDSurfaceDesc->lpSurface = it->second.data.data();
                    }
                    else
                    {
                        lpDDSurfaceDesc->lpSurface = buffers.emplace(std::piecewise_construct, std::forward_as_tuple(lpDestRect),
                                std::forward_as_tuple(lpDDSurfaceDesc->lpSurface, lpDDSurfaceDesc->dwWidth, lpDDSurfaceDesc->dwHeight)
                            ).first->second.data.data();
                    }
                    log(Logger::Level::Trace) << "Lock() finished.";
                }
            }
        }
        return result;
    }

    virtual __stdcall HRESULT ReleaseDC(HDC arg)
    {
        log() << "ReleaseDC.";
        return underlying->ReleaseDC(arg);
    }

    virtual __stdcall HRESULT Restore()
    {
        log() << "Restore.";
        return underlying->Restore();
    }

    virtual __stdcall HRESULT SetClipper(LPDIRECTDRAWCLIPPER arg)
    {
        log() << "SetClipper.";
        return underlying->SetClipper(arg);
    }

    virtual __stdcall HRESULT SetColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        log() << "SetColorKey.";
        return underlying->SetColorKey(arg1, arg2);
    }

    virtual __stdcall HRESULT SetOverlayPosition(LONG arg1, LONG arg2)
    {
        log() << "SetOverlayPosition.";
        return underlying->SetOverlayPosition(arg1, arg2);
    }

    virtual __stdcall HRESULT SetPalette(LPDIRECTDRAWPALETTE arg)
    {
        log() << "SetPalette.";
        return underlying->SetPalette(arg);
    }

    virtual __stdcall HRESULT Unlock(LPRECT lpRect)
    {
        if (Constants::Emulate16BitsPerPixel && is_primary)
        {
            auto it = buffers.find(lpRect);
            /// If application is properly using DirectDraw, it will point to element.
            /// Otherwise, we have to make some assumptions (see below).
            if (it == buffers.end() && lpRect != nullptr)
            {
                /// Okay, let's check if we have entire buffer.
                auto entire_it = buffers.find(nullptr);
                if (lpRect->top == 0 && lpRect->left == 0 && entire_it != buffers.end()
                    && (entire_it->second.width == lpRect->right && entire_it->second.height == lpRect->bottom
                        || lpRect->right == 0 && lpRect->bottom == 0))
                {
                    it = entire_it;    
                }
            }
            if (Constants::AllowTrashInSurfaceUnlockRect && it == buffers.end() && buffers.size() == 1)
            {
                /// Okay, if we have only one buffer, use it.
                it = buffers.begin();
            }
            if (it != buffers.end())
            {
                log(Logger::Level::Trace) << "Unlock() started.";

                auto now = std::chrono::system_clock::now();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_render_time).count() > 20
                    || !Constants::AllowToUseUnlockedSurfaceMemory || !it->first.is_null)
                {
                    it->second.render();
                    last_render_time = now;
                }

                if (!Constants::AllowToUseUnlockedSurfaceMemory || !it->first.is_null)
                {
                    /// Won't erase entire buffer because an application may write to it before next Lock(). //_-
                    buffers.erase(it);
                }
            }
            else
            {
                log(Logger::Level::Error) << "Unlocking unknown rect.";
            }
        }
        log(Logger::Level::Trace) << "Unlock(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", rect=" << (lpRect == nullptr ? std::string("NULL")
                : std::string("{") + std::to_string(lpRect->left) + ", " + std::to_string(lpRect->top) + ", "
                + std::to_string(lpRect->right) + ", " + std::to_string(lpRect->bottom) + "}") << ").";
        return underlying->Unlock(lpRect);
    }

    virtual __stdcall HRESULT UpdateOverlay(LPRECT arg1, LPDIRECTDRAWSURFACE7 arg2, LPRECT arg3, DWORD arg4, LPDDOVERLAYFX arg5)
    {
        log() << "UpdateOverlay.";
        return underlying->UpdateOverlay(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT UpdateOverlayDisplay(DWORD arg)
    {
        log() << "UpdateOverlayDisplay.";
        return underlying->UpdateOverlayDisplay(arg);
    }

    virtual __stdcall HRESULT UpdateOverlayZOrder(DWORD arg1, LPDIRECTDRAWSURFACE7 arg2)
    {
        log() << "UpdateOverlayZOrder.";
        return underlying->UpdateOverlayZOrder(arg1, surface_holder.getOriginalSurface(arg2));
    }

    virtual __stdcall HRESULT GetDDInterface(LPVOID FAR * lplpDD)
    {
        log() << "GetDDInterface.";
        LPVOID lpDD;
        HRESULT result = underlying->GetDDInterface(&lpDD);
        *lplpDD = static_cast<LPVOID>(&surface_holder);
        return result;
    }

    virtual __stdcall HRESULT PageLock(DWORD arg)
    {
        log() << "PageLock.";
        return underlying->PageLock(arg);
    }

    virtual __stdcall HRESULT PageUnlock(DWORD arg)
    {
        log() << "PageUnlock.";
        return underlying->PageUnlock(arg);
    }

    virtual __stdcall HRESULT SetSurfaceDesc(LPDDSURFACEDESC2 arg1, DWORD arg2)
    {
        log() << "SetSurfaceDesc.";
        return underlying->SetSurfaceDesc(arg1, arg2);
    }

    virtual __stdcall HRESULT SetPrivateData(REFGUID arg1, LPVOID arg2, DWORD arg3, DWORD arg4)
    {
        log() << "SetPrivateData.";
        return underlying->SetPrivateData(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT GetPrivateData(REFGUID arg1, LPVOID arg2, LPDWORD arg3)
    {
        log() << "GetPrivateData.";
        return underlying->GetPrivateData(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT FreePrivateData(REFGUID arg)
    {
        log() << "FreePrivateData.";
        return underlying->FreePrivateData(arg);
    }

    virtual __stdcall HRESULT GetUniquenessValue(LPDWORD arg)
    {
        log() << "GetUniquenessValue.";
        return underlying->GetUniquenessValue(arg);
    }

    virtual __stdcall HRESULT ChangeUniquenessValue()
    {
        log() << "ChangeUniquenessValue.";
        return underlying->ChangeUniquenessValue();
    }

    virtual __stdcall HRESULT SetPriority(DWORD arg)
    {
        log() << "SetPriority.";
        return underlying->SetPriority(arg);
    }

    virtual __stdcall HRESULT GetPriority(LPDWORD arg)
    {
        log() << "GetPriority.";
        return underlying->GetPriority(arg);
    }

    virtual __stdcall HRESULT SetLOD(DWORD arg)
    {
        log() << "SetLOD.";
        return underlying->SetLOD(arg);
    }

    virtual __stdcall HRESULT GetLOD(LPDWORD arg)
    {
        log() << "GetLOD.";
        return underlying->GetLOD(arg);
    }
};
