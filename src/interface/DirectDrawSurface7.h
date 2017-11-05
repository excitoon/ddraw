#pragma once

#include <windows.h>
#include <unwindows.h>

#include <interface/Unknown.h>


namespace interface
{

class DirectDrawSurface7 : virtual public Unknown
{
public:
    using SurfaceDescription = DDSURFACEDESC2;
    using SurfaceCapabilities = DDSCAPS2;
    using EnumSurfacesCallback = __stdcall PASCAL HRESULT (*)(DirectDrawSurface7 *, SurfaceDescription *, void *);

    virtual __stdcall HRESULT AddAttachedSurface(DirectDrawSurface7 * arg) = 0;
    virtual __stdcall HRESULT AddOverlayDirtyRect(LPRECT arg) = 0;
    virtual __stdcall HRESULT Blt(LPRECT arg1, DirectDrawSurface7 * arg2, LPRECT arg3, DWORD arg4, LPDDBLTFX arg5) = 0;
    virtual __stdcall HRESULT BltBatch(LPDDBLTBATCH arg1, DWORD arg2, DWORD arg3) = 0;
    virtual __stdcall HRESULT BltFast(DWORD arg1, DWORD arg2, DirectDrawSurface7 * arg3, LPRECT arg4, DWORD arg5) = 0;
    virtual __stdcall HRESULT DeleteAttachedSurface(DWORD arg1, DirectDrawSurface7 * arg2) = 0;
    virtual __stdcall HRESULT EnumAttachedSurfaces(LPVOID arg1, EnumSurfacesCallback arg2) = 0;
    virtual __stdcall HRESULT EnumOverlayZOrders(DWORD arg1, LPVOID arg2, EnumSurfacesCallback arg3) = 0;
    virtual __stdcall HRESULT Flip(DirectDrawSurface7 * arg1, DWORD arg2) = 0;
    virtual __stdcall HRESULT GetAttachedSurface(SurfaceCapabilities * arg1, DirectDrawSurface7 * FAR * arg2) = 0;
    virtual __stdcall HRESULT GetBltStatus(DWORD arg) = 0;
    virtual __stdcall HRESULT GetCaps(SurfaceCapabilities * arg) = 0;
    virtual __stdcall HRESULT GetClipper(LPDIRECTDRAWCLIPPER FAR * arg) = 0;
    virtual __stdcall HRESULT GetColorKey(DWORD arg1, LPDDCOLORKEY arg2) = 0;
    virtual __stdcall HRESULT GetDC(HDC FAR * arg) = 0;
    virtual __stdcall HRESULT GetFlipStatus(DWORD arg) = 0;
    virtual __stdcall HRESULT GetOverlayPosition(LPLONG arg1, LPLONG arg2) = 0;
    virtual __stdcall HRESULT GetPalette(LPDIRECTDRAWPALETTE FAR * arg) = 0;
    virtual __stdcall HRESULT GetPixelFormat(LPDDPIXELFORMAT arg) = 0;
    virtual __stdcall HRESULT GetSurfaceDesc(SurfaceDescription * arg) = 0;
    virtual __stdcall HRESULT Initialize(LPDIRECTDRAW arg1, SurfaceDescription * arg2) = 0;
    virtual __stdcall HRESULT IsLost() = 0;
    virtual __stdcall HRESULT Lock(LPRECT arg1, SurfaceDescription * arg2, DWORD arg3, HANDLE arg4) = 0;
    virtual __stdcall HRESULT ReleaseDC(HDC arg) = 0;
    virtual __stdcall HRESULT Restore() = 0;
    virtual __stdcall HRESULT SetClipper(LPDIRECTDRAWCLIPPER arg) = 0;
    virtual __stdcall HRESULT SetColorKey(DWORD arg1, LPDDCOLORKEY arg2) = 0;
    virtual __stdcall HRESULT SetOverlayPosition(LONG arg1, LONG arg2) = 0;
    virtual __stdcall HRESULT SetPalette(LPDIRECTDRAWPALETTE arg) = 0;
    virtual __stdcall HRESULT Unlock(LPRECT arg) = 0;
    virtual __stdcall HRESULT UpdateOverlay(LPRECT arg1, DirectDrawSurface7 * arg2, LPRECT arg3, DWORD arg4, LPDDOVERLAYFX arg5) = 0;
    virtual __stdcall HRESULT UpdateOverlayDisplay(DWORD arg) = 0;
    virtual __stdcall HRESULT UpdateOverlayZOrder(DWORD arg1, DirectDrawSurface7 * arg2) = 0;
    virtual __stdcall HRESULT GetDDInterface(LPVOID FAR * arg) = 0;
    virtual __stdcall HRESULT PageLock(DWORD arg) = 0;
    virtual __stdcall HRESULT PageUnlock(DWORD arg) = 0;
    virtual __stdcall HRESULT SetSurfaceDesc(SurfaceDescription * arg1, DWORD arg2) = 0;
    virtual __stdcall HRESULT SetPrivateData(REFGUID arg1, LPVOID arg2, DWORD arg3, DWORD arg4) = 0;
    virtual __stdcall HRESULT GetPrivateData(REFGUID arg1, LPVOID arg2, LPDWORD arg3) = 0;
    virtual __stdcall HRESULT FreePrivateData(REFGUID arg) = 0;
    virtual __stdcall HRESULT GetUniquenessValue(LPDWORD arg) = 0;
    virtual __stdcall HRESULT ChangeUniquenessValue() = 0;
    virtual __stdcall HRESULT SetPriority(DWORD arg) = 0;
    virtual __stdcall HRESULT GetPriority(LPDWORD arg) = 0;
    virtual __stdcall HRESULT SetLOD(DWORD arg) = 0;
    virtual __stdcall HRESULT GetLOD(LPDWORD arg) = 0;
};

}
