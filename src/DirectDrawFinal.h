#pragma once

#include <unordered_map>
#include <stdexcept>

#include <windows.h>

#include <interface/DirectDraw.h>
#include <interface/DirectDrawSurface.h>
#include <proxy/DirectDrawSurface.h>
#include "Logger.h"
#include "Constants.h"
#include "DirectDrawSurfaceFinal.h"
#include "Scheduler.h"
#include "UnknownFinal.h"


template <template <typename Final> typename Proxy>
class DirectDrawFinal : public Proxy<DirectDrawFinal<Proxy>>, public UnknownFinal<DirectDrawFinal<Proxy>, interface::DirectDraw>
{
    using DirectDraw = DirectDrawFinal<Proxy>;
    using OriginalDirectDraw = interface::DirectDraw;
    using DirectDrawCapabilities = DDCAPS;
    using Surface = DirectDrawSurfaceFinal<proxy::DirectDrawSurface, DirectDraw>;
    using OriginalSurface = interface::DirectDrawSurface;
    using SurfaceDescription = DDSURFACEDESC;
    using SurfaceCapabilities = DDSCAPS;

    Scheduler & scheduler;
    OriginalDirectDraw * underlying;
    std::unordered_map<OriginalSurface *, Surface *> wrapped_surfaces;
    std::unordered_map<Surface *, OriginalSurface *> original_surfaces;
    /// TODO. Clear them on IDirectDrawSurface::Release().

    Logger log = Logger("DirectDraw");

    template <typename MyInterface>
    MyInterface & getUnderlying()
    {
        return *underlying;
    }

public:
    DirectDrawFinal(Scheduler & scheduler, OriginalDirectDraw * underlying):
        UnknownFinal<DirectDraw, OriginalDirectDraw>(scheduler, underlying),
        scheduler(scheduler),
        underlying(underlying)
    {
    }

    ~DirectDrawFinal()
    {
    }

    OriginalSurface * getWrappedSurface(OriginalSurface * original_surface, bool is_primary = false)
    {
        if (original_surface != nullptr)
        {
            if (wrapped_surfaces.count(original_surface) > 0)
            {
                Surface * surface = wrapped_surfaces[original_surface];
                return surface;
            }
            else
            {
                Surface * surface = new Surface(scheduler, *this, original_surface, is_primary);
                /// TODO. Wrapper surfaces are not being deleted sometimes, when last Release() is called in
                /// DirectX code. Some DirectX calls require special handling to fix that.
                /// I don't call AddRef() on original interfaces when it is needed in some cases.

                wrapped_surfaces[original_surface] = surface;
                original_surfaces[surface] = original_surface;
                return surface;
            }
        }
        else
        {
            return original_surface;
        }
    }

    OriginalSurface * getOriginalSurface(OriginalSurface * wrapped_surface)
    {
        if (wrapped_surface == nullptr)
        {
            return wrapped_surface;
        }

        Surface * surface = static_cast<Surface *>(wrapped_surface);
        if (original_surfaces.count(surface) > 0)
        {
            return original_surfaces[surface];
        }
        else
        {
            log(LogLevel::Error) << "Wrong surface.";
            return wrapped_surface;
        }
    }

    HRESULT compact()
    {
        log() << "Compact.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Compact(); });
    }

    HRESULT createClipper(DWORD arg1, LPDIRECTDRAWCLIPPER FAR * arg2, IUnknown FAR * arg3)
    {
        log() << "CreateClipper.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->CreateClipper(arg1, arg2, arg3); });
    }

    HRESULT createPalette(DWORD arg1, LPPALETTEENTRY arg2, LPDIRECTDRAWPALETTE FAR * arg3, IUnknown FAR * arg4)
    {
        log() << "CreatePalette.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->CreatePalette(arg1, arg2, arg3, arg4); });
    }

    HRESULT createSurface(SurfaceDescription * description, OriginalSurface * FAR * lplpDDSurface, IUnknown FAR * pUnkOuter)
    {
        OriginalSurface * lpDDSurface = nullptr;
        bool is_primary_surface = description->dwFlags & DDSD_CAPS && description->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE;
        if (!is_primary_surface && Constants::Emulate16BitsPerPixel)
        {
            description->dwFlags |= DDSD_PIXELFORMAT;
            description->ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
            description->ddpfPixelFormat.dwFlags = DDPF_RGB;
            description->ddpfPixelFormat.dwRGBBitCount = 16;
            description->ddpfPixelFormat.dwRBitMask = 0xF800;
            description->ddpfPixelFormat.dwGBitMask = 0x07E0; /// 5:6:5
            description->ddpfPixelFormat.dwBBitMask = 0x001F;
            description->ddpfPixelFormat.dwRGBAlphaBitMask = 0;
        }
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->CreateSurface(description, &lpDDSurface, pUnkOuter); });
        /// TODO. Save description to surface.
        *lplpDDSurface = getWrappedSurface(lpDDSurface, is_primary_surface);
        log() << "CreateSurface(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << description->dwFlags << std::dec
            << ", width=" << description->dwWidth << ", height=" << description->dwHeight
            << ", caps=" << std::hex << std::setfill('0') << std::setw(8) << description->ddsCaps.dwCaps << std::dec << ")"
            << " -> " << std::hex << std::setfill('0') << std::setw(8) << *lplpDDSurface << std::dec << ".";
        return result;
    }

    HRESULT duplicateSurface(OriginalSurface * lpDDSurface, OriginalSurface * FAR * lplpDupDDSurface)
    {
        log() << "DuplicateSurface.";
        OriginalSurface * lpDupDDSurface = nullptr;
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->DuplicateSurface(getOriginalSurface(lpDDSurface), &lpDupDDSurface); });
        *lplpDupDDSurface = getWrappedSurface(lpDupDDSurface);
        return result;
    }

    template <typename Callback>
    HRESULT enumDisplayModes(DWORD arg1, SurfaceDescription * arg2, LPVOID arg3, Callback arg4)
    {
        log() << "EnumDisplayModes.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->EnumDisplayModes(arg1, arg2, arg3, arg4); });
    }

    template <typename Callback>
    struct WrappedContext
    {
        LPVOID lpContext;
        Callback lpEnumSurfacesCallback;
        DWORD dwFlags;
        DirectDraw & direct_draw;

        static __stdcall HRESULT enumSurfacesCallback(OriginalSurface * lpDDSurface, SurfaceDescription * lpDDSurfaceDesc, LPVOID lpContext)
        {
            WrappedContext<Callback> & context = *static_cast<WrappedContext<Callback> *>(lpContext);
            context.direct_draw.log() << "EnumSurfacesCallback.";
            return context.lpEnumSurfacesCallback(context.direct_draw.getWrappedSurface(lpDDSurface), lpDDSurfaceDesc, context.lpContext);
        }
    };

    template <typename Callback>
    HRESULT enumSurfaces(DWORD dwFlags, SurfaceDescription * description, LPVOID lpContext, Callback callback)
    {
        log() << "EnumSurfaces.";
        WrappedContext<Callback> wrapped_context{ lpContext, callback, dwFlags, *this };
        return scheduler.makeTask<HRESULT>([&]() { return underlying->EnumSurfaces(dwFlags, description, static_cast<void *>(&wrapped_context), WrappedContext<Callback>::enumSurfacesCallback); });
    }

    HRESULT flipToGDISurface()
    {
        log() << "FlipToGDISurface.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->FlipToGDISurface(); });
    }

    HRESULT getCaps(DirectDrawCapabilities * arg1, DirectDrawCapabilities * arg2)
    {
        log() << "GetCaps() started.";
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->GetCaps(arg1, arg2); });
        log() << "GetCaps() finished, result=" << result << ".";
        return result;
    }

    HRESULT getDisplayMode(SurfaceDescription * arg)
    {
        log() << "GetDisplayMode.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetDisplayMode(arg); });
    }

    HRESULT getFourCCCodes(LPDWORD arg1, LPDWORD arg2)
    {
        log() << "GetFourCCCodes.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetFourCCCodes(arg1, arg2); });
    }

    HRESULT getGDISurface(OriginalSurface * FAR * lplpGDIDDSSurface)
    {
        log() << "GetGDISurface.";
        OriginalSurface * lpGDIDDSSurface = nullptr;
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->GetGDISurface(&lpGDIDDSSurface); });
        *lplpGDIDDSSurface = getWrappedSurface(lpGDIDDSSurface);
        return result;
    }

    HRESULT getMonitorFrequency(LPDWORD arg)
    {
        log() << "GetMonitorFrequency.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetMonitorFrequency(arg); });
    }

    HRESULT getScanLine(LPDWORD arg)
    {
        log() << "GetScanLine.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetScanLine(arg); });
    }

    HRESULT getVerticalBlankStatus(LPBOOL arg)
    {
        log() << "GetVerticalBlankStatus.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->GetVerticalBlankStatus(arg); });
    }

    HRESULT initialize(GUID FAR * arg)
    {
        log() << "Initialize.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->Initialize(arg); });
    }

    HRESULT restoreDisplayMode()
    {
        log() << "RestoreDisplayMode.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->RestoreDisplayMode(); });
    }

    HRESULT setCooperativeLevel(HWND hWnd, DWORD dwFlags)
    {
        log() << "SetCooperativeLevel(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", hwnd=" << std::hex << std::setfill('0') << std::setw(8) << hWnd << std::dec
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << dwFlags << std::dec << ") started.";
        if (Constants::DisableExclusiveCooperativeLevel)
        {
            if (dwFlags & DDSCL_EXCLUSIVE)
            {
                dwFlags = (dwFlags & ~DDSCL_EXCLUSIVE & ~DDSCL_FULLSCREEN & ~DDSCL_ALLOWMODEX) | DDSCL_NORMAL;
            }
        }
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->SetCooperativeLevel(hWnd, dwFlags); });
        log() << "SetCooperativeLevel() finished, result=" << result << ".";
        return result;
    }

    template <typename... Args>
    HRESULT setDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, Args... args)
    {
        log() << "SetDisplayMode(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", width=" << dwWidth << ", height=" << dwHeight << ", bpp=" << dwBPP << ") started.";
        if (Constants::Emulate16BitsPerPixel)
        {
            if (dwBPP == 16)
            {
                dwBPP = 32;
            }
        }
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return underlying->SetDisplayMode(dwWidth, dwHeight, dwBPP, args...); });
        log() << "SetDisplayMode() finished, result=" << result << ".";
        return result;
    }

    HRESULT waitForVerticalBlank(DWORD arg1, HANDLE arg2)
    {
        log() << "WaitForVerticalBlank.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->WaitForVerticalBlank(arg1, arg2); });
    }

    template <typename MyInterface = OriginalDirectDraw>
    HRESULT getAvailableVidMem(SurfaceCapabilities * arg1, LPDWORD arg2, LPDWORD arg3)
    {
        log() << "GetAvailableVidMem.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().GetAvailableVidMem(arg1, arg2, arg3); });
    }

    template <typename MyInterface = OriginalDirectDraw>
    HRESULT getSurfaceFromDC(HDC hdc, OriginalSurface ** lplpDDS)
    {
        log() << "GetSurfaceFromDC.";
        OriginalSurface * lpDDS = nullptr;
        HRESULT result = scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().GetSurfaceFromDC(hdc, &lpDDS); });
        *lplpDDS = getWrappedSurface(lpDDS);
        return result;
    }

    template <typename MyInterface = OriginalDirectDraw>
    HRESULT restoreAllSurfaces()
    {
        log() << "RestoreAllSurfaces.";
        return scheduler.makeTask<HRESULT>([&]() { return  getUnderlying<MyInterface>().RestoreAllSurfaces(); });
    }

    template <typename MyInterface = OriginalDirectDraw>
    HRESULT testCooperativeLevel()
    {
        log() << "TestCooperativeLevel.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().TestCooperativeLevel(); });
    }

    template <typename MyInterface = OriginalDirectDraw>
    HRESULT getDeviceIdentifier(LPDDDEVICEIDENTIFIER2 arg1, DWORD arg2)
    {
        log() << "GetDeviceIdentifier.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().GetDeviceIdentifier(arg1, arg2); });
    }

    template <typename MyInterface = OriginalDirectDraw>
    HRESULT startModeTest(LPSIZE arg1, DWORD arg2, DWORD arg3)
    {
        log() << "StartModeTest.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().StartModeTest(arg1, arg2, arg3); });
    }

    template <typename MyInterface = OriginalDirectDraw>
    HRESULT evaluateMode(DWORD arg1, DWORD * arg2)
    {
        log() << "EvaluateMode.";
        return scheduler.makeTask<HRESULT>([&]() { return getUnderlying<MyInterface>().EvaluateMode(arg1, arg2); });
    }
};
