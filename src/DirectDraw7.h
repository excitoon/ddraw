#pragma once

#include <unordered_map>
#include <stdexcept>

#include <windows.h>

#include "Unknown.h"
#include "Logger.h"
#include "DirectDrawSurface7.h"


class DirectDraw7 : public Unknown<DirectDraw7>
{
    IDirectDraw7 * underlying;
    std::unordered_map<LPDIRECTDRAWSURFACE7, DirectDrawSurface7<DirectDraw7> *> wrapped_surfaces;
    std::unordered_map<DirectDrawSurface7<DirectDraw7> *, LPDIRECTDRAWSURFACE7> original_surfaces;
    /// TODO: Clear them on IDirectDrawSurface7::Release().

    bool disable_exclusive_cooperative_level = true;
    bool emulate_16_bits_per_pixel = true;

	Logger log = Logger(Logger::Level::Trace, "DirectDraw7");

public:
    DirectDraw7(IDirectDraw7 * underlying):
        Unknown<DirectDraw7>(underlying),
        underlying(underlying)
    {
    }

    ~DirectDraw7()
    {
    }

    LPDIRECTDRAWSURFACE7 getWrappedSurface(LPDIRECTDRAWSURFACE7 lpDDSurface, bool is_primary = false)
    {
        if (lpDDSurface != nullptr)
        {
            if (wrapped_surfaces.count(lpDDSurface) > 0)
            {
                DirectDrawSurface7<DirectDraw7> * surface = wrapped_surfaces[lpDDSurface];
                return reinterpret_cast<LPDIRECTDRAWSURFACE7>(surface);
            }
            else
            {
                DirectDrawSurface7<DirectDraw7> * surface = new DirectDrawSurface7<DirectDraw7>(*this, lpDDSurface, emulate_16_bits_per_pixel, is_primary);
                /// TODO: Wrapper surfaces are not being deleted sometimes, when last Release() is called in
                /// DirectX code. Some DirectX calls require special handling to fix that.
                /// I don't call AddRef() on original interfaces when it is needed in some cases.

                wrapped_surfaces[lpDDSurface] = surface;
                original_surfaces[surface] = lpDDSurface;
                return reinterpret_cast<LPDIRECTDRAWSURFACE7>(surface);
            }
        }
        else
        {
            return lpDDSurface;
        }
    }

    LPDIRECTDRAWSURFACE7 getOriginalSurface(LPDIRECTDRAWSURFACE7 lpDDSurface)
    {
        if (lpDDSurface == nullptr)
        {
            return lpDDSurface;
        }

        DirectDrawSurface7<DirectDraw7> * surface = reinterpret_cast<DirectDrawSurface7<DirectDraw7> *>(lpDDSurface);
        if (original_surfaces.count(surface) > 0)
        {
            return reinterpret_cast<LPDIRECTDRAWSURFACE7>(original_surfaces[surface]);
        }
        else
        {
            /// Logic error, wrong surface.
            return lpDDSurface;
        }
    }

    virtual __stdcall HRESULT Compact()
    {
        log() << "Compact.";
        return underlying->Compact();
    }

    virtual __stdcall HRESULT CreateClipper(DWORD arg1, LPDIRECTDRAWCLIPPER FAR * arg2, IUnknown FAR * arg3)
    {
        log() << "CreateClipper.";
        return underlying->CreateClipper(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT CreatePalette(DWORD arg1, LPPALETTEENTRY arg2, LPDIRECTDRAWPALETTE FAR * arg3, IUnknown FAR * arg4)
    {
        log() << "CreatePalette.";
        return underlying->CreatePalette(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT CreateSurface(LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPDIRECTDRAWSURFACE7 FAR * lplpDDSurface, IUnknown FAR * pUnkOuter)
    {
        log() << "CreateSurface(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << lpDDSurfaceDesc2->dwFlags << std::dec
            << ", width=" << lpDDSurfaceDesc2->dwWidth << ", height=" << lpDDSurfaceDesc2->dwHeight
            << ", caps=" << std::hex << std::setfill('0') << std::setw(8) << lpDDSurfaceDesc2->ddsCaps.dwCaps << std::dec << ").";
        LPDIRECTDRAWSURFACE7 lpDDSurface = nullptr;
        bool is_primary_surface = lpDDSurfaceDesc2->dwFlags & DDSD_CAPS && lpDDSurfaceDesc2->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE;
        HRESULT result = underlying->CreateSurface(lpDDSurfaceDesc2, &lpDDSurface, pUnkOuter);
        *lplpDDSurface = getWrappedSurface(lpDDSurface, is_primary_surface);
        return result;
    }

    virtual __stdcall HRESULT DuplicateSurface(LPDIRECTDRAWSURFACE7 lpDDSurface, LPDIRECTDRAWSURFACE7 FAR * lplpDupDDSurface)
    {
        log() << "DuplicateSurface.";
        LPDIRECTDRAWSURFACE7 lpDupDDSurface = nullptr;
        HRESULT result = underlying->DuplicateSurface(getOriginalSurface(lpDDSurface), &lpDupDDSurface);
        *lplpDupDDSurface = getWrappedSurface(lpDupDDSurface);
        return result;
    }

    virtual __stdcall HRESULT EnumDisplayModes(DWORD arg1, LPDDSURFACEDESC2 arg2, LPVOID arg3, LPDDENUMMODESCALLBACK2 arg4)
    {
        log() << "EnumDisplayModes.";
        return underlying->EnumDisplayModes(arg1, arg2, arg3, arg4);
    }

    struct WrapperContext
    {
        LPVOID lpContext;
        LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback;
        DWORD dwFlags;
        DirectDraw7 & direct_draw7;
    };

    static __stdcall HRESULT EnumSurfacesCallback7(LPDIRECTDRAWSURFACE7 lpDDSurface, LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext)
    {
        WrapperContext & context = *static_cast<WrapperContext *>(lpContext);
        context.direct_draw7.log() << "EnumSurfacesCallback7.";
        return context.lpEnumSurfacesCallback(context.direct_draw7.getWrappedSurface(lpDDSurface), lpDDSurfaceDesc, context.lpContext);
    }

    virtual __stdcall HRESULT EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC2 lpDDSD2, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback)
    {
        log() << "EnumSurfaces.";
        WrapperContext wrapper_context{ lpContext, lpEnumSurfacesCallback, dwFlags, *this };
        return underlying->EnumSurfaces(dwFlags, lpDDSD2, static_cast<LPVOID>(&wrapper_context), EnumSurfacesCallback7);
    }

    virtual __stdcall HRESULT FlipToGDISurface()
    {
        log() << "FlipToGDISurface.";
        return underlying->FlipToGDISurface();
    }

    virtual __stdcall HRESULT GetCaps(LPDDCAPS arg1, LPDDCAPS arg2)
    {
        log() << "GetCaps.";
        return underlying->GetCaps(arg1, arg2);
    }

    virtual __stdcall HRESULT GetDisplayMode(LPDDSURFACEDESC2 arg)
    {
        log() << "GetDisplayMode.";
        return underlying->GetDisplayMode(arg);
    }

    virtual __stdcall HRESULT GetFourCCCodes(LPDWORD arg1, LPDWORD arg2)
    {
        log() << "GetFourCCCodes.";
        return underlying->GetFourCCCodes(arg1, arg2);
    }

    virtual __stdcall HRESULT GetGDISurface(LPDIRECTDRAWSURFACE7 FAR * lplpGDIDDSSurface)
    {
        log() << "GetGDISurface.";
        LPDIRECTDRAWSURFACE7 lpGDIDDSSurface = nullptr;
        HRESULT result = underlying->GetGDISurface(&lpGDIDDSSurface);
        *lplpGDIDDSSurface = getWrappedSurface(lpGDIDDSSurface);
        return result;
    }

    virtual __stdcall HRESULT GetMonitorFrequency(LPDWORD arg)
    {
        log() << "GetMonitorFrequency.";
        return underlying->GetMonitorFrequency(arg);
    }

    virtual __stdcall HRESULT GetScanLine(LPDWORD arg)
    {
        log() << "GetScanLine.";
        return underlying->GetScanLine(arg);
    }

    virtual __stdcall HRESULT GetVerticalBlankStatus(LPBOOL arg)
    {
        log() << "GetVerticalBlankStatus.";
        return underlying->GetVerticalBlankStatus(arg);
    }

    virtual __stdcall HRESULT Initialize(GUID FAR * arg)
    {
        log() << "Initialize.";
        return underlying->Initialize(arg);
    }

    virtual __stdcall HRESULT RestoreDisplayMode()
    {
        log() << "RestoreDisplayMode.";
        return underlying->RestoreDisplayMode();
    }

    virtual __stdcall HRESULT SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
    {
        log() << "SetCooperativeLevel(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", hwnd=" << std::hex << std::setfill('0') << std::setw(8) << hWnd << std::dec
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << dwFlags << std::dec << ").";
        if (disable_exclusive_cooperative_level)
        {
            if (dwFlags & DDSCL_EXCLUSIVE)
            {
                dwFlags = (dwFlags & ~DDSCL_EXCLUSIVE) | DDSCL_NORMAL;
            }
        }
        return underlying->SetCooperativeLevel(hWnd, dwFlags);
    }

    virtual __stdcall HRESULT SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate, DWORD dwFlags)
    {
        log() << "SetDisplayMode(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", width=" << dwWidth << ", height=" << dwHeight << ", bpp=" << dwBPP << ", refreshRate=" << dwRefreshRate
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << dwFlags << std::dec << ").";
        if (emulate_16_bits_per_pixel)
        {
            if (dwBPP == 16)
            {
                dwBPP = 32;
            }
        }
        return underlying->SetDisplayMode(dwWidth, dwHeight, dwBPP, dwRefreshRate, dwFlags);
    }

    virtual __stdcall HRESULT WaitForVerticalBlank(DWORD arg1, HANDLE arg2)
    {
        log() << "WaitForVerticalBlank.";
        return underlying->WaitForVerticalBlank(arg1, arg2);
    }

    virtual __stdcall HRESULT GetAvailableVidMem(LPDDSCAPS2 arg1, LPDWORD arg2, LPDWORD arg3)
    {
        log() << "GetAvailableVidMem.";
        return underlying->GetAvailableVidMem(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT GetSurfaceFromDC(HDC hdc, LPDIRECTDRAWSURFACE7 * lplpDDS)
    {
        log() << "GetSurfaceFromDC.";
        LPDIRECTDRAWSURFACE7 lpDDS = nullptr;
        HRESULT result = underlying->GetSurfaceFromDC(hdc, &lpDDS);
        *lplpDDS = getWrappedSurface(lpDDS);
        return result;
    }

    virtual __stdcall HRESULT RestoreAllSurfaces()
    {
        log() << "RestoreAllSurfaces.";
        return underlying->RestoreAllSurfaces();
    }

    virtual __stdcall HRESULT TestCooperativeLevel()
    {
        log() << "TestCooperativeLevel.";
        return underlying->TestCooperativeLevel();
    }

    virtual __stdcall HRESULT GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2 arg1, DWORD arg2)
    {
        log() << "GetDeviceIdentifier.";
        return underlying->GetDeviceIdentifier(arg1, arg2);
    }

    virtual __stdcall HRESULT StartModeTest(LPSIZE arg1, DWORD arg2, DWORD arg3)
    {
        log() << "StartModeTest.";
        return underlying->StartModeTest(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT EvaluateMode(DWORD arg1, DWORD * arg2)
    {
        log() << "EvaluateMode.";
        return underlying->EvaluateMode(arg1, arg2);
    }
};
