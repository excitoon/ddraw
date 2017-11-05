#pragma once

#include <windows.h>
#include <unwindows.h>

#include <interface/Unknown.h>


namespace interface
{

class DirectDrawSurface4;

class DirectDraw4 : virtual public Unknown
{
public:
    using DirectDrawCapabilities = DDCAPS;
    using Surface = DirectDrawSurface4;
    using SurfaceDescription = DDSURFACEDESC2;
    using SurfaceCapabilities = DDSCAPS2;
    using EnumSurfacesCallback = __stdcall PASCAL HRESULT (*)(DirectDrawSurface4 *, SurfaceDescription *, void *);

    virtual __stdcall HRESULT Compact() = 0;
    virtual __stdcall HRESULT CreateClipper(DWORD arg1, LPDIRECTDRAWCLIPPER FAR * arg2, IUnknown FAR * arg3) = 0;
    virtual __stdcall HRESULT CreatePalette(DWORD arg1, LPPALETTEENTRY arg2, LPDIRECTDRAWPALETTE FAR * arg3, IUnknown FAR * arg4) = 0;
    virtual __stdcall HRESULT CreateSurface(SurfaceDescription * arg1, DirectDrawSurface4 * FAR * arg2, IUnknown FAR * arg3) = 0;
    virtual __stdcall HRESULT DuplicateSurface(DirectDrawSurface4 * arg1, DirectDrawSurface4 * FAR * arg2) = 0;
    virtual __stdcall HRESULT EnumDisplayModes(DWORD arg1, SurfaceDescription * arg2, LPVOID arg3, LPDDENUMMODESCALLBACK2 arg4) = 0;
    virtual __stdcall HRESULT EnumSurfaces(DWORD arg1, SurfaceDescription * arg2, LPVOID arg3, EnumSurfacesCallback arg4) = 0;
    virtual __stdcall HRESULT FlipToGDISurface() = 0;
    virtual __stdcall HRESULT GetCaps(DirectDrawCapabilities * arg1, DirectDrawCapabilities * arg2) = 0;
    virtual __stdcall HRESULT GetDisplayMode(SurfaceDescription * arg) = 0;
    virtual __stdcall HRESULT GetFourCCCodes(LPDWORD arg1, LPDWORD arg2) = 0;
    virtual __stdcall HRESULT GetGDISurface(DirectDrawSurface4 * FAR * arg) = 0;
    virtual __stdcall HRESULT GetMonitorFrequency(LPDWORD arg) = 0;
    virtual __stdcall HRESULT GetScanLine(LPDWORD arg) = 0;
    virtual __stdcall HRESULT GetVerticalBlankStatus(LPBOOL arg) = 0;
    virtual __stdcall HRESULT Initialize(GUID FAR * arg) = 0;
    virtual __stdcall HRESULT RestoreDisplayMode() = 0;
    virtual __stdcall HRESULT SetCooperativeLevel(HWND arg1, DWORD arg2) = 0;
    virtual __stdcall HRESULT SetDisplayMode(DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5) = 0;
    virtual __stdcall HRESULT WaitForVerticalBlank(DWORD arg1, HANDLE arg2) = 0;
    virtual __stdcall HRESULT GetAvailableVidMem(SurfaceCapabilities * arg1, LPDWORD arg2, LPDWORD arg3) = 0;
    virtual __stdcall HRESULT GetSurfaceFromDC(HDC arg1, DirectDrawSurface4 ** arg2) = 0;
    virtual __stdcall HRESULT RestoreAllSurfaces() = 0;
    virtual __stdcall HRESULT TestCooperativeLevel() = 0;
    virtual __stdcall HRESULT GetDeviceIdentifier(LPDDDEVICEIDENTIFIER arg1, DWORD arg2) = 0;
};

}
