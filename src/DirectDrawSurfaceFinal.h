#pragma once

#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <utility>

#include <windows.h>

#include "Buffer.h"
#include "Constants.h"
#include "LogLevel.h"
#include "Logger.h"
#include "Scheduler.h"
#include "UnknownFinal.h"


template <template <typename Final> typename Proxy, typename SurfaceHolder>
class DirectDrawSurfaceFinal : public Proxy<DirectDrawSurfaceFinal<Proxy, SurfaceHolder>>,
        public UnknownFinal<DirectDrawSurfaceFinal<Proxy, SurfaceHolder>, typename SurfaceHolder::OriginalSurface>
{
    using Surface = DirectDrawSurfaceFinal<Proxy, SurfaceHolder>;
    using OriginalSurface = typename SurfaceHolder::OriginalSurface;
    using SurfaceDescription = typename OriginalSurface::SurfaceDescription;
    using SurfaceCapabilities = typename OriginalSurface::SurfaceCapabilities;

    Scheduler & scheduler;
    OriginalSurface * underlying;
    SurfaceHolder & surface_holder;

    bool is_primary;

    using SurfaceBuffer = Buffer<OriginalSurface>;
    std::unordered_map<typename SurfaceBuffer::Key, SurfaceBuffer, typename SurfaceBuffer::Key::Hasher> buffers;
    std::unordered_map<unsigned char *, typename SurfaceBuffer::Key> ptr_rect;

    Logger log = Logger("DirectDrawSurface");

    template <typename MyInterface>
    MyInterface & getUnderlying()
    {
        return *underlying;
    }

public:
    DirectDrawSurfaceFinal(Scheduler & scheduler, SurfaceHolder & surface_holder, OriginalSurface * underlying, bool is_primary):
        UnknownFinal<Surface, OriginalSurface>(scheduler, underlying),
        scheduler(scheduler),
        underlying(underlying),
        surface_holder(surface_holder),
        is_primary(is_primary)
    {
    }

    ~DirectDrawSurfaceFinal()
    {
    }

    SurfaceHolder & getSurfaceHolder()
    {
        return surface_holder;
    }

    HRESULT addAttachedSurface(OriginalSurface * arg)
    {
        log() << "AddAttachedSurface.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->AddAttachedSurface(surface_holder.getOriginalSurface(arg)); });
    }

    HRESULT addOverlayDirtyRect(LPRECT arg)
    {
        log() << "AddOverlayDirtyRect.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->AddOverlayDirtyRect(arg); });
    }

    HRESULT blt(LPRECT arg1, OriginalSurface * arg2, LPRECT arg3, DWORD arg4, LPDDBLTFX arg5)
    {
        log() << "Blt.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Blt(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5); });
    }

    HRESULT bltBatch(LPDDBLTBATCH arg1, DWORD arg2, DWORD arg3)
    {
        log() << "BltBatch.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->BltBatch(arg1, arg2, arg3); });
    }

    HRESULT bltFast(DWORD arg1, DWORD arg2, OriginalSurface * arg3, LPRECT arg4, DWORD arg5)
    {
        log() << "BltFast.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->BltFast(arg1, arg2, surface_holder.getOriginalSurface(arg3), arg4, arg5); });
    }

    HRESULT deleteAttachedSurface(DWORD arg1, OriginalSurface * arg2)
    {
        log() << "DeleteAttachedSurface.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->DeleteAttachedSurface(arg1, surface_holder.getOriginalSurface(arg2)); });
    }

    template <typename Callback>
    struct WrappedContext
    {
        LPVOID lpContext;
        Callback lpEnumSurfacesCallback;
        DWORD dwFlags;
        Surface & surface;

        static __stdcall HRESULT enumSurfacesCallback(OriginalSurface * lpDDSurface, SurfaceDescription * lpDDSurfaceDesc, LPVOID lpContext)
        {
            WrappedContext<Callback> & context = *static_cast<WrappedContext<Callback> *>(lpContext);
            context.surface.log() << "EnumSurfacesCallback.";
            return context.lpEnumSurfacesCallback(context.surface.getSurfaceHolder().getWrappedSurface(lpDDSurface), lpDDSurfaceDesc, context.lpContext);
        }
    };

    template <typename Callback>
    HRESULT enumAttachedSurfaces(LPVOID lpContext, Callback lpEnumSurfacesCallback)
    {
        log() << "EnumAttachedSurfaces.";
        WrappedContext<Callback> wrapped_context{ lpContext, lpEnumSurfacesCallback, 0, *this };
        return scheduler.makeTask<HRESULT>([&]() { return underlying->EnumAttachedSurfaces(static_cast<LPVOID>(&wrapped_context), WrappedContext<Callback>::enumSurfacesCallback); });
    }

    template <typename Callback>
    HRESULT enumOverlayZOrders(DWORD dwFlags, LPVOID lpContext, Callback lpfnCallback)
    {
        log() << "EnumOverlayZOrders.";
        WrappedContext<Callback> wrapped_context{ lpContext, lpfnCallback, dwFlags, *this };
        return scheduler.makeTask<HRESULT>([&]() { return underlying->EnumOverlayZOrders(dwFlags, static_cast<LPVOID>(&wrapped_context), WrappedContext<Callback>::enumSurfacesCallback); });
    }

    HRESULT flip(OriginalSurface * arg1, DWORD arg2)
    {
        log() << "Flip.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Flip(surface_holder.getOriginalSurface(arg1), arg2); });
    }

    HRESULT getAttachedSurface(SurfaceCapabilities * lpDDSCaps, OriginalSurface * FAR * lplpDDAttachedSurface)
    {
        log() << "GetAttachedSurface.";
        OriginalSurface * lpDDAttachedSurface = nullptr;
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->GetAttachedSurface(lpDDSCaps, &lpDDAttachedSurface); });
        *lplpDDAttachedSurface = surface_holder.getWrappedSurface(lpDDAttachedSurface);
        return result;
    }

    HRESULT getBltStatus(DWORD arg)
    {
        log() << "GetBltStatus.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetBltStatus(arg); });
    }

    HRESULT getCaps(SurfaceCapabilities * arg)
    {
        log() << "GetCaps.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetCaps(arg); });
    }

    HRESULT getClipper(LPDIRECTDRAWCLIPPER FAR * arg)
    {
        log() << "GetClipper.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetClipper(arg); });
    }

    HRESULT getColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        log() << "GetColorKey.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetColorKey(arg1, arg2); });
    }

    HRESULT getDC(HDC FAR * arg)
    {
        log() << "GetDC.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetDC(arg); });
    }

    HRESULT getFlipStatus(DWORD arg)
    {
        log() << "GetFlipStatus.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetFlipStatus(arg); });
    }

    HRESULT getOverlayPosition(LPLONG arg1, LPLONG arg2)
    {
        log() << "GetOverlayPosition.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetOverlayPosition(arg1, arg2); });
    }

    HRESULT getPalette(LPDIRECTDRAWPALETTE FAR * arg)
    {
        log() << "GetPalette.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetPalette(arg); });
    }

    HRESULT getPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
    {
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->GetPixelFormat(lpDDPixelFormat); });
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

    HRESULT getSurfaceDesc(SurfaceDescription * arg)
    {
        log() << "GetSurfaceDesc.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetSurfaceDesc(arg); });
    }

    HRESULT initialize(LPDIRECTDRAW arg1, SurfaceDescription * arg2)
    {
        log() << "Initialize.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Initialize(arg1, arg2); });
    }

    HRESULT isLost()
    {
        log(LogLevel::Trace) << "IsLost.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->IsLost(); });
    }

    HRESULT lock(LPRECT lpDestRect, SurfaceDescription * lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
    {
        log(LogLevel::Trace) << "Lock() started.";
        HRESULT result;
        if (Constants::Emulate16BitsPerPixel && is_primary)
        {
            if (Constants::EnablePrimarySurfaceBackgroundBuffering && lpDestRect == nullptr)
            {
                /// In background rendering we only do first Lock() to obtain data.
                /// At this point there is no rendering task in action.
                auto it = buffers.find(lpDestRect);
                if (it != buffers.end())
                {
                    /// Do nothing.
                    /// N.B. Copy only part of surface description which client application can understood
                    /// because we don't want to ruin their memory.
                    std::copy(reinterpret_cast<unsigned char *>(&it->second.surface_description),
                            reinterpret_cast<unsigned char *>(&it->second.surface_description) + it->second.surface_description.dwSize,
                            reinterpret_cast<unsigned char *>(lpDDSurfaceDesc));
                    it->second.fillSurfaceDescription(*lpDDSurfaceDesc);
                    result = DD_OK;
                }
                else
                {
                    result = scheduler.makeTask<HRESULT>([&]() { return underlying->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent); });
                    if (result == DD_OK && lpDDSurfaceDesc->dwFlags & DDSD_WIDTH && lpDDSurfaceDesc->dwFlags & DDSD_HEIGHT)
                    {
                        auto it = buffers.emplace(std::piecewise_construct, std::forward_as_tuple(lpDestRect),
                                std::forward_as_tuple(&scheduler, underlying, lpDDSurfaceDesc, dwFlags)).first;
                        ptr_rect.emplace(it->second.data.data(), it->first);
                        it->second.fillSurfaceDescription(*lpDDSurfaceDesc);
                        HRESULT result2 = scheduler.makeTask<HRESULT>([&]() { return underlying->Unlock(lpDestRect); });
                        /// If we failed to Unlock() we're doomed.
                        if (result2 != DD_OK)
                        {
                            log(LogLevel::Error) << "Unlock() failed.";
                        }
                    }
                }
            }
            else
            {
                result = scheduler.makeTask<HRESULT>([&]() { return underlying->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent); });
                if (result == DD_OK && lpDDSurfaceDesc->dwFlags & DDSD_WIDTH && lpDDSurfaceDesc->dwFlags & DDSD_HEIGHT)
                {
                    auto it = buffers.find(lpDestRect);
                    if (it != buffers.end())
                    {
                        /// In foreground rendering we delete buffer each Unlock().
                        log(LogLevel::Error) << "This is impossible situation.";
                    }
                    else
                    {
                        it = buffers.emplace(std::piecewise_construct, std::forward_as_tuple(lpDestRect),
                                std::forward_as_tuple(&scheduler, underlying, lpDDSurfaceDesc, dwFlags)).first;
                        ptr_rect.emplace(it->second.data.data(), it->first);
                        it->second.fillSurfaceDescription(*lpDDSurfaceDesc);
                    }
                }
            }
        }
        else
        {
            result = scheduler.makeTask<HRESULT>([&]() { return underlying->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent); });
            if (Constants::Emulate16BitsPerPixel && lpDDSurfaceDesc != nullptr)
            {
                lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount = 16;
                lpDDSurfaceDesc->ddpfPixelFormat.dwRBitMask = 0xF800;
                lpDDSurfaceDesc->ddpfPixelFormat.dwGBitMask = 0x07E0; /// 5:6:5
                lpDDSurfaceDesc->ddpfPixelFormat.dwBBitMask = 0x001F;
                lpDDSurfaceDesc->ddpfPixelFormat.dwRGBAlphaBitMask = 0;
            }
        }
        log(LogLevel::Trace) << "Lock(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", rect=" << std::hex << std::setfill('0') << std::setw(8) << lpDestRect << std::dec
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << dwFlags << std::dec << ")"
            << " -> width=" << lpDDSurfaceDesc->dwWidth << ", height=" << lpDDSurfaceDesc->dwHeight
            << ", pitch=" << lpDDSurfaceDesc->lPitch << ", bpp=" << lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << lpDDSurfaceDesc->dwFlags << std::dec << ".";
        return result;
    }

    HRESULT releaseDC(HDC arg)
    {
        log() << "ReleaseDC.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->ReleaseDC(arg); });
    }

    HRESULT restore()
    {
        log() << "Restore.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Restore(); });
    }

    HRESULT setClipper(LPDIRECTDRAWCLIPPER arg)
    {
        log() << "SetClipper.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetClipper(arg); });
    }

    HRESULT setColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        log() << "SetColorKey.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetColorKey(arg1, arg2); });
    }

    HRESULT setOverlayPosition(LONG arg1, LONG arg2)
    {
        log() << "SetOverlayPosition.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetOverlayPosition(arg1, arg2); });
    }

    HRESULT setPalette(LPDIRECTDRAWPALETTE arg)
    {
        log() << "SetPalette.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetPalette(arg); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT unlock(LPVOID lpSurface)
    {
        auto it = ptr_rect.find(static_cast<unsigned char *>(lpSurface));
        if (it != ptr_rect.end())
        {
            return unlock(&it->second.rect);
        }
        log(LogLevel::Trace) << "Unlock(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", lpSurface=" << std::hex << std::setfill('0') << std::setw(8) << lpSurface << std::dec << ").";
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().Unlock(lpSurface); });
        log(LogLevel::Trace) << "Unlock() finished.";
        return result;
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT unlock(LPRECT lpRect)
    {
        log(LogLevel::Trace) << "Unlock(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", rect=" << (lpRect == nullptr ? std::string("NULL")
                : std::string("{") + std::to_string(lpRect->left) + ", " + std::to_string(lpRect->top) + ", "
                + std::to_string(lpRect->right) + ", " + std::to_string(lpRect->bottom) + "}") << ").";
        HRESULT result;
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
            if (it != buffers.end())
            {
                if (Constants::EnablePrimarySurfaceBackgroundBuffering && it->first.is_null)
                {
                    it->second.renderInBackground();
                    result = DD_OK;
                }
                else
                {
                    it->second.render();
                    ptr_rect.erase(it->second.data.data());
                    buffers.erase(it);
                    result = scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().Unlock(lpRect); });
                }
            }
            else
            {
                log(LogLevel::Error) << "Unlocking unknown rect.";
                result = scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().Unlock(lpRect); });
            }
        }
        else
        {
            result = scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().Unlock(lpRect); });
        }
        log(LogLevel::Trace) << "Unlock() finished.";
        return result;
    }

    HRESULT updateOverlay(LPRECT arg1, OriginalSurface * arg2, LPRECT arg3, DWORD arg4, LPDDOVERLAYFX arg5)
    {
        log() << "UpdateOverlay.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->UpdateOverlay(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5); });
    }

    HRESULT updateOverlayDisplay(DWORD arg)
    {
        log() << "UpdateOverlayDisplay.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->UpdateOverlayDisplay(arg); });
    }

    HRESULT updateOverlayZOrder(DWORD arg1, OriginalSurface * arg2)
    {
        log() << "UpdateOverlayZOrder.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->UpdateOverlayZOrder(arg1, surface_holder.getOriginalSurface(arg2)); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT getDDInterface(LPVOID FAR * lplpDD)
    {
        log() << "GetDDInterface.";
        LPVOID lpDD;
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().GetDDInterface(&lpDD); });
        *lplpDD = static_cast<LPVOID>(&surface_holder);
        return result;
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT pageLock(DWORD arg)
    {
        log() << "PageLock.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().PageLock(arg); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT pageUnlock(DWORD arg)
    {
        log() << "PageUnlock.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().PageUnlock(arg); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT setSurfaceDesc(SurfaceDescription * arg1, DWORD arg2)
    {
        log() << "SetSurfaceDesc.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().SetSurfaceDesc(arg1, arg2); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT setPrivateData(REFGUID arg1, LPVOID arg2, DWORD arg3, DWORD arg4)
    {
        log() << "SetPrivateData.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().SetPrivateData(arg1, arg2, arg3, arg4); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT getPrivateData(REFGUID arg1, LPVOID arg2, LPDWORD arg3)
    {
        log() << "GetPrivateData.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().GetPrivateData(arg1, arg2, arg3); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT freePrivateData(REFGUID arg)
    {
        log() << "FreePrivateData.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().FreePrivateData(arg); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT getUniquenessValue(LPDWORD arg)
    {
        log() << "GetUniquenessValue.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().GetUniquenessValue(arg); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT changeUniquenessValue()
    {
        log() << "ChangeUniquenessValue.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().ChangeUniquenessValue(); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT setPriority(DWORD arg)
    {
        log() << "SetPriority.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().SetPriority(arg); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT getPriority(LPDWORD arg)
    {
        log() << "GetPriority.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().GetPriority(arg); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT setLOD(DWORD arg)
    {
        log() << "SetLOD.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().SetLOD(arg); });
    }

    template <typename MyInterface = OriginalSurface>
    HRESULT getLOD(LPDWORD arg)
    {
        log() << "GetLOD.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().GetLOD(arg); });
    }
};
