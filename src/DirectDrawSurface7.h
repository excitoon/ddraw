#pragma once

#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <utility>

#include <windows.h>

#include "Buffer.h"
#include "Constants.h"
#include "Logger.h"
#include "Scheduler.h"
#include "Unknown.h"


template <typename SurfaceHolder>
class DirectDrawSurface7 : public Unknown<DirectDrawSurface7<SurfaceHolder>>
{
    Scheduler & scheduler;
    IDirectDrawSurface7 * underlying;
    SurfaceHolder & surface_holder;

    bool is_primary;

    std::unordered_map<Buffer::Key, Buffer, Buffer::Key::Hasher> buffers;

    Logger log = Logger(Logger::Level::Trace, "DirectDrawSurface7");

public:
    DirectDrawSurface7(Scheduler & scheduler, SurfaceHolder & surface_holder, IDirectDrawSurface7 * underlying, bool is_primary):
        Unknown<DirectDrawSurface7>(scheduler, underlying),
        scheduler(scheduler),
        underlying(underlying),
        surface_holder(surface_holder),
        is_primary(is_primary)
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
        return scheduler.makeTask<HRESULT>([&]() { return underlying->AddAttachedSurface(surface_holder.getOriginalSurface(arg)); });
    }

    virtual __stdcall HRESULT AddOverlayDirtyRect(LPRECT arg)
    {
        log() << "AddOverlayDirtyRect.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->AddOverlayDirtyRect(arg); });
    }

    virtual __stdcall HRESULT Blt(LPRECT arg1, LPDIRECTDRAWSURFACE7 arg2, LPRECT arg3, DWORD arg4, LPDDBLTFX arg5)
    {
        log() << "Blt.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Blt(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5); });
    }

    virtual __stdcall HRESULT BltBatch(LPDDBLTBATCH arg1, DWORD arg2, DWORD arg3)
    {
        log() << "BltBatch.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->BltBatch(arg1, arg2, arg3); });
    }

    virtual __stdcall HRESULT BltFast(DWORD arg1, DWORD arg2, LPDIRECTDRAWSURFACE7 arg3, LPRECT arg4, DWORD arg5)
    {
        log() << "BltFast.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->BltFast(arg1, arg2, surface_holder.getOriginalSurface(arg3), arg4, arg5); });
    }

    virtual __stdcall HRESULT DeleteAttachedSurface(DWORD arg1, LPDIRECTDRAWSURFACE7 arg2)
    {
        log() << "DeleteAttachedSurface.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->DeleteAttachedSurface(arg1, surface_holder.getOriginalSurface(arg2)); });
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
        return scheduler.makeTask<HRESULT>([&]() { return underlying->EnumAttachedSurfaces(static_cast<LPVOID>(&wrapper_context), EnumSurfacesCallback7); });
    }

    virtual __stdcall HRESULT EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpfnCallback)
    {
        log() << "EnumOverlayZOrders.";
        WrapperContext wrapper_context{ lpContext, lpfnCallback, dwFlags, *this };
        return scheduler.makeTask<HRESULT>([&]() { return underlying->EnumOverlayZOrders(dwFlags, static_cast<LPVOID>(&wrapper_context), EnumSurfacesCallback7); });
    }

    virtual __stdcall HRESULT Flip(LPDIRECTDRAWSURFACE7 arg1, DWORD arg2)
    {
        log() << "Flip.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Flip(surface_holder.getOriginalSurface(arg1), arg2); });
    }

    virtual __stdcall HRESULT GetAttachedSurface(LPDDSCAPS2 lpDDSCaps, LPDIRECTDRAWSURFACE7 FAR * lplpDDAttachedSurface)
    {
        log() << "GetAttachedSurface.";
        LPDIRECTDRAWSURFACE7 lpDDAttachedSurface = nullptr;
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->GetAttachedSurface(lpDDSCaps, &lpDDAttachedSurface); });
        *lplpDDAttachedSurface = surface_holder.getWrappedSurface(lpDDAttachedSurface);
        return result;
    }

    virtual __stdcall HRESULT GetBltStatus(DWORD arg)
    {
        log() << "GetBltStatus.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetBltStatus(arg); });
    }

    virtual __stdcall HRESULT GetCaps(LPDDSCAPS2 arg)
    {
        log() << "GetCaps.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetCaps(arg); });
    }

    virtual __stdcall HRESULT GetClipper(LPDIRECTDRAWCLIPPER FAR * arg)
    {
        log() << "GetClipper.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetClipper(arg); });
    }

    virtual __stdcall HRESULT GetColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        log() << "GetColorKey.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetColorKey(arg1, arg2); });
    }

    virtual __stdcall HRESULT GetDC(HDC FAR * arg)
    {
        log() << "GetDC.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetDC(arg); });
    }

    virtual __stdcall HRESULT GetFlipStatus(DWORD arg)
    {
        log() << "GetFlipStatus.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetFlipStatus(arg); });
    }

    virtual __stdcall HRESULT GetOverlayPosition(LPLONG arg1, LPLONG arg2)
    {
        log() << "GetOverlayPosition.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetOverlayPosition(arg1, arg2); });
    }

    virtual __stdcall HRESULT GetPalette(LPDIRECTDRAWPALETTE FAR * arg)
    {
        log() << "GetPalette.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetPalette(arg); });
    }

    virtual __stdcall HRESULT GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
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

    virtual __stdcall HRESULT GetSurfaceDesc(LPDDSURFACEDESC2 arg)
    {
        log() << "GetSurfaceDesc.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetSurfaceDesc(arg); });
    }

    virtual __stdcall HRESULT Initialize(LPDIRECTDRAW arg1, LPDDSURFACEDESC2 arg2)
    {
        log() << "Initialize.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Initialize(arg1, arg2); });
    }

    virtual __stdcall HRESULT IsLost()
    {
        log() << "IsLost.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->IsLost(); });
    }

    virtual __stdcall HRESULT Lock(LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
    {
        log(Logger::Level::Trace) << "Lock() started.";
        HRESULT result;
        if (Constants::Emulate16BitsPerPixel && is_primary)
        {
            if (Constants::AllowToUseUnlockedSurfaceMemory && lpDestRect == nullptr)
            {
                /// In background rendering we only do first Lock() to obtain data.
                /// At this point there is no rendering task in action.
                auto it = buffers.find(lpDestRect);
                if (it != buffers.end())
                {
                    /// Do nothing.
                    /// N.B. Copy only part of surface description which client application can understood
                    /// because we don't want to ruin their memory.
                    std::copy(reinterpret_cast<unsigned char *>(&it->second.DDSurfaceDesc),
                            reinterpret_cast<unsigned char *>(&it->second.DDSurfaceDesc) + it->second.DDSurfaceDesc.dwSize,
                            reinterpret_cast<unsigned char *>(lpDDSurfaceDesc));
                    lpDDSurfaceDesc->lpSurface = it->second.data.data();
                    result = DD_OK;
                }
                else
                {
                    result = scheduler.makeTask<HRESULT>([&]() { return underlying->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent); });
                    if (result == DD_OK && lpDDSurfaceDesc->dwFlags & DDSD_WIDTH && lpDDSurfaceDesc->dwFlags & DDSD_HEIGHT)
                    {
                        lpDDSurfaceDesc->lpSurface = buffers.emplace(std::piecewise_construct, std::forward_as_tuple(lpDestRect),
                                std::forward_as_tuple(&scheduler, underlying, lpDDSurfaceDesc, dwFlags)).first->second.data.data();
                        HRESULT result2 = scheduler.makeTask<HRESULT>([&]() { return underlying->Unlock(lpDestRect); });
                        /// If we failed to Unlock() we're doomed.
                        if (result2 != DD_OK)
                        {
                            log(Logger::Level::Error) << "Unlock() failed.";
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
                        if (Constants::AllowToUseUnlockedSurfaceMemory && it->first.is_null)
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
                    }
                    else
                    {
                        lpDDSurfaceDesc->lpSurface = buffers.emplace(std::piecewise_construct, std::forward_as_tuple(lpDestRect),
                                std::forward_as_tuple(&scheduler, underlying, lpDDSurfaceDesc, dwFlags)).first->second.data.data();
                    }
                }
            }
        }
        else
        {
            result = scheduler.makeTask<HRESULT>([&]() { return underlying->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent); });
        }
        log(Logger::Level::Trace) << "Lock(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", rect=" << std::hex << std::setfill('0') << std::setw(8) << lpDestRect << std::dec
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << dwFlags << std::dec << ")"
            << " -> width=" << lpDDSurfaceDesc->dwWidth << ", height=" << lpDDSurfaceDesc->dwHeight
            << ", pitch=" << lpDDSurfaceDesc->lPitch << ", bpp=" << lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << lpDDSurfaceDesc->dwFlags << std::dec << ".";
        return result;
    }

    virtual __stdcall HRESULT ReleaseDC(HDC arg)
    {
        log() << "ReleaseDC.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->ReleaseDC(arg); });
    }

    virtual __stdcall HRESULT Restore()
    {
        log() << "Restore.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Restore(); });
    }

    virtual __stdcall HRESULT SetClipper(LPDIRECTDRAWCLIPPER arg)
    {
        log() << "SetClipper.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetClipper(arg); });
    }

    virtual __stdcall HRESULT SetColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        log() << "SetColorKey.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetColorKey(arg1, arg2); });
    }

    virtual __stdcall HRESULT SetOverlayPosition(LONG arg1, LONG arg2)
    {
        log() << "SetOverlayPosition.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetOverlayPosition(arg1, arg2); });
    }

    virtual __stdcall HRESULT SetPalette(LPDIRECTDRAWPALETTE arg)
    {
        log() << "SetPalette.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetPalette(arg); });
    }

    virtual __stdcall HRESULT Unlock(LPRECT lpRect)
    {
        log(Logger::Level::Trace) << "Unlock(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
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
            if (Constants::AllowTrashInSurfaceUnlockRect && it == buffers.end() && buffers.size() == 1)
            {
                /// Okay, if we have only one buffer, use it.
                it = buffers.begin();
            }
            if (it != buffers.end())
            {
                if (Constants::AllowToUseUnlockedSurfaceMemory && it->first.is_null)
                {
                    it->second.renderInBackground();
                    result = DD_OK;
                }
                else
                {
                    it->second.render();
                    buffers.erase(it);
                    result = scheduler.makeTask<HRESULT>([&]() { return underlying->Unlock(lpRect); });
                }
            }
            else
            {
                log(Logger::Level::Error) << "Unlocking unknown rect.";
                result = scheduler.makeTask<HRESULT>([&]() { return underlying->Unlock(lpRect); });
            }
        }
        else
        {
            result = scheduler.makeTask<HRESULT>([&]() { return underlying->Unlock(lpRect); });
        }
        log(Logger::Level::Trace) << "Unlock() finished.";
        return result;
    }

    virtual __stdcall HRESULT UpdateOverlay(LPRECT arg1, LPDIRECTDRAWSURFACE7 arg2, LPRECT arg3, DWORD arg4, LPDDOVERLAYFX arg5)
    {
        log() << "UpdateOverlay.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->UpdateOverlay(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5); });
    }

    virtual __stdcall HRESULT UpdateOverlayDisplay(DWORD arg)
    {
        log() << "UpdateOverlayDisplay.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->UpdateOverlayDisplay(arg); });
    }

    virtual __stdcall HRESULT UpdateOverlayZOrder(DWORD arg1, LPDIRECTDRAWSURFACE7 arg2)
    {
        log() << "UpdateOverlayZOrder.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->UpdateOverlayZOrder(arg1, surface_holder.getOriginalSurface(arg2)); });
    }

    virtual __stdcall HRESULT GetDDInterface(LPVOID FAR * lplpDD)
    {
        log() << "GetDDInterface.";
        LPVOID lpDD;
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->GetDDInterface(&lpDD); });
        *lplpDD = static_cast<LPVOID>(&surface_holder);
        return result;
    }

    virtual __stdcall HRESULT PageLock(DWORD arg)
    {
        log() << "PageLock.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->PageLock(arg); });
    }

    virtual __stdcall HRESULT PageUnlock(DWORD arg)
    {
        log() << "PageUnlock.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->PageUnlock(arg); });
    }

    virtual __stdcall HRESULT SetSurfaceDesc(LPDDSURFACEDESC2 arg1, DWORD arg2)
    {
        log() << "SetSurfaceDesc.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetSurfaceDesc(arg1, arg2); });
    }

    virtual __stdcall HRESULT SetPrivateData(REFGUID arg1, LPVOID arg2, DWORD arg3, DWORD arg4)
    {
        log() << "SetPrivateData.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetPrivateData(arg1, arg2, arg3, arg4); });
    }

    virtual __stdcall HRESULT GetPrivateData(REFGUID arg1, LPVOID arg2, LPDWORD arg3)
    {
        log() << "GetPrivateData.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetPrivateData(arg1, arg2, arg3); });
    }

    virtual __stdcall HRESULT FreePrivateData(REFGUID arg)
    {
        log() << "FreePrivateData.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->FreePrivateData(arg); });
    }

    virtual __stdcall HRESULT GetUniquenessValue(LPDWORD arg)
    {
        log() << "GetUniquenessValue.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetUniquenessValue(arg); });
    }

    virtual __stdcall HRESULT ChangeUniquenessValue()
    {
        log() << "ChangeUniquenessValue.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->ChangeUniquenessValue(); });
    }

    virtual __stdcall HRESULT SetPriority(DWORD arg)
    {
        log() << "SetPriority.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetPriority(arg); });
    }

    virtual __stdcall HRESULT GetPriority(LPDWORD arg)
    {
        log() << "GetPriority.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetPriority(arg); });
    }

    virtual __stdcall HRESULT SetLOD(DWORD arg)
    {
        log() << "SetLOD.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->SetLOD(arg); });
    }

    virtual __stdcall HRESULT GetLOD(LPDWORD arg)
    {
        log() << "GetLOD.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetLOD(arg); });
    }
};
