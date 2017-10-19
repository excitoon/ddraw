#pragma once

#include <windows.h>

#include "Unknown.h"


class DirectDraw7 : public Unknown<DirectDraw7>
{
    IDirectDraw7 * underlying;

public:
    virtual __stdcall HRESULT Compact()
    {
        return underlying->Compact();
    }

    virtual __stdcall HRESULT CreateClipper(DWORD arg1, LPDIRECTDRAWCLIPPER FAR * arg2, IUnknown FAR * arg3)
    {
        return underlying->CreateClipper(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT CreatePalette(DWORD arg1, LPPALETTEENTRY arg2, LPDIRECTDRAWPALETTE FAR * arg3, IUnknown FAR * arg4)
    {
        return underlying->CreatePalette(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT CreateSurface(LPDDSURFACEDESC2 arg1, LPDIRECTDRAWSURFACE7 FAR * arg2, IUnknown FAR * arg3)
    {
        return underlying->CreateSurface(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT DuplicateSurface(LPDIRECTDRAWSURFACE7 arg1, LPDIRECTDRAWSURFACE7 FAR * arg2)
    {
        return underlying->DuplicateSurface(arg1, arg2);
    }

    virtual __stdcall HRESULT EnumDisplayModes(DWORD arg1, LPDDSURFACEDESC2 arg2, LPVOID arg3, LPDDENUMMODESCALLBACK2 arg4)
    {
        return underlying->EnumDisplayModes(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT EnumSurfaces(DWORD arg1, LPDDSURFACEDESC2 arg2, LPVOID arg3, LPDDENUMSURFACESCALLBACK7 arg4)
    {
        return underlying->EnumSurfaces(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT FlipToGDISurface()
    {
        return underlying->FlipToGDISurface();
    }

    virtual __stdcall HRESULT GetCaps(LPDDCAPS arg1, LPDDCAPS arg2)
    {
        return underlying->GetCaps(arg1, arg2);
    }

    virtual __stdcall HRESULT GetDisplayMode(LPDDSURFACEDESC2 arg)
    {
        return underlying->GetDisplayMode(arg);
    }

    virtual __stdcall HRESULT GetFourCCCodes(LPDWORD arg1, LPDWORD arg2)
    {
        return underlying->GetFourCCCodes(arg1, arg2);
    }

    virtual __stdcall HRESULT GetGDISurface(LPDIRECTDRAWSURFACE7 FAR * arg)
    {
        return underlying->GetGDISurface(arg);
    }

    virtual __stdcall HRESULT GetMonitorFrequency(LPDWORD arg)
    {
        return underlying->GetMonitorFrequency(arg);
    }

    virtual __stdcall HRESULT GetScanLine(LPDWORD arg)
    {
        return underlying->GetScanLine(arg);
    }

    virtual __stdcall HRESULT GetVerticalBlankStatus(LPBOOL arg)
    {
        return underlying->GetVerticalBlankStatus(arg);
    }

    virtual __stdcall HRESULT Initialize(GUID FAR * arg)
    {
        return underlying->Initialize(arg);
    }

    virtual __stdcall HRESULT RestoreDisplayMode()
    {
        return underlying->RestoreDisplayMode();
    }

    virtual __stdcall HRESULT SetCooperativeLevel(HWND arg1, DWORD arg2)
    {
        return underlying->SetCooperativeLevel(arg1, arg2);
    }

    virtual __stdcall HRESULT SetDisplayMode(DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5)
    {
        return underlying->SetDisplayMode(arg1, arg2, arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT WaitForVerticalBlank(DWORD arg1, HANDLE arg2)
    {
        return underlying->WaitForVerticalBlank(arg1, arg2);
    }

    virtual __stdcall HRESULT GetAvailableVidMem(LPDDSCAPS2 arg1, LPDWORD arg2, LPDWORD arg3)
    {
        return underlying->GetAvailableVidMem(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT GetSurfaceFromDC(HDC arg1, LPDIRECTDRAWSURFACE7 * arg2)
    {
        return underlying->GetSurfaceFromDC(arg1, arg2);
    }

    virtual __stdcall HRESULT RestoreAllSurfaces()
    {
        return underlying->RestoreAllSurfaces();
    }

    virtual __stdcall HRESULT TestCooperativeLevel()
    {
        return underlying->TestCooperativeLevel();
    }

    virtual __stdcall HRESULT GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2 arg1, DWORD arg2)
    {
        return underlying->GetDeviceIdentifier(arg1, arg2);
    }

    virtual __stdcall HRESULT StartModeTest(LPSIZE arg1, DWORD arg2, DWORD arg3)
    {
        return underlying->StartModeTest(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT EvaluateMode(DWORD arg1, DWORD * arg2)
    {
        return underlying->EvaluateMode(arg1, arg2);
    }

    DirectDraw7(IDirectDraw7 * underlying):
        Unknown<DirectDraw7>(underlying),
        underlying(underlying)
    {
    }

    ~DirectDraw7()
    {
    }
};
