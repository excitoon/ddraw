#pragma once

#include <windows.h>
#include <unwindows.h>

#include <interface/Unknown.h>


namespace interface
{

class DirectDrawSurface7;

class DirectDraw7 : virtual public Unknown
{
public:
    using EnumSurfacesCallback = __stdcall PASCAL HRESULT (*)(DirectDrawSurface7 *, DDSURFACEDESC2 *, void *);

    virtual __stdcall HRESULT Compact() = 0;
    virtual __stdcall HRESULT CreateClipper(DWORD arg1, LPDIRECTDRAWCLIPPER FAR * arg2, IUnknown FAR * arg3) = 0;
    virtual __stdcall HRESULT CreatePalette(DWORD arg1, LPPALETTEENTRY arg2, LPDIRECTDRAWPALETTE FAR * arg3, IUnknown FAR * arg4) = 0;
    virtual __stdcall HRESULT CreateSurface(LPDDSURFACEDESC2 arg1, DirectDrawSurface7 * FAR * arg2, IUnknown FAR * arg3) = 0;
    virtual __stdcall HRESULT DuplicateSurface(DirectDrawSurface7 * arg1, DirectDrawSurface7 * FAR * arg2) = 0;
    virtual __stdcall HRESULT EnumDisplayModes(DWORD arg1, LPDDSURFACEDESC2 arg2, LPVOID arg3, LPDDENUMMODESCALLBACK2 arg4) = 0;
    virtual __stdcall HRESULT EnumSurfaces(DWORD arg1, LPDDSURFACEDESC2 arg2, LPVOID arg3, EnumSurfacesCallback arg4) = 0;
    virtual __stdcall HRESULT FlipToGDISurface() = 0;
    virtual __stdcall HRESULT GetCaps(LPDDCAPS arg1, LPDDCAPS arg2) = 0;
    virtual __stdcall HRESULT GetDisplayMode(LPDDSURFACEDESC2 arg) = 0;
    virtual __stdcall HRESULT GetFourCCCodes(LPDWORD arg1, LPDWORD arg2) = 0;
    virtual __stdcall HRESULT GetGDISurface(DirectDrawSurface7 * FAR * arg) = 0;
    virtual __stdcall HRESULT GetMonitorFrequency(LPDWORD arg) = 0;
    virtual __stdcall HRESULT GetScanLine(LPDWORD arg) = 0;
    virtual __stdcall HRESULT GetVerticalBlankStatus(LPBOOL arg) = 0;
    virtual __stdcall HRESULT Initialize(GUID FAR * arg) = 0;
    virtual __stdcall HRESULT RestoreDisplayMode() = 0;
    virtual __stdcall HRESULT SetCooperativeLevel(HWND arg1, DWORD arg2) = 0;
    virtual __stdcall HRESULT SetDisplayMode(DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5) = 0;
    virtual __stdcall HRESULT WaitForVerticalBlank(DWORD arg1, HANDLE arg2) = 0;
    virtual __stdcall HRESULT GetAvailableVidMem(LPDDSCAPS2 arg1, LPDWORD arg2, LPDWORD arg3) = 0;
    virtual __stdcall HRESULT GetSurfaceFromDC(HDC arg1, DirectDrawSurface7 ** arg2) = 0;
    virtual __stdcall HRESULT RestoreAllSurfaces() = 0;
    virtual __stdcall HRESULT TestCooperativeLevel() = 0;
    virtual __stdcall HRESULT GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2 arg1, DWORD arg2) = 0;
    virtual __stdcall HRESULT StartModeTest(LPSIZE arg1, DWORD arg2, DWORD arg3) = 0;
    virtual __stdcall HRESULT EvaluateMode(DWORD arg1, DWORD * arg2) = 0;
};

}
