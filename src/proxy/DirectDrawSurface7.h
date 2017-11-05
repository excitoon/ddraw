#pragma once

#include <windows.h>

#include <interface/DirectDrawSurface7.h>
#include <proxy/Unknown.h>


namespace proxy
{

template <typename Final>
class DirectDrawSurface7 : public interface::DirectDrawSurface7, public Unknown<Final>
{
    Final & getFinal()
    {
        return static_cast<Final &>(*this);
    }

public:
    using Interface = interface::DirectDrawSurface7;

    virtual __stdcall HRESULT AddAttachedSurface(Interface * arg) override final
    {
        return getFinal().addAttachedSurface(arg);
    }

    virtual __stdcall HRESULT AddOverlayDirtyRect(LPRECT arg) override final
    {
        return getFinal().addOverlayDirtyRect(arg);
    }

    virtual __stdcall HRESULT Blt(LPRECT arg1, Interface * arg2, LPRECT arg3, DWORD arg4, LPDDBLTFX arg5) override final
    {
        return getFinal().blt(arg1, arg2, arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT BltBatch(LPDDBLTBATCH arg1, DWORD arg2, DWORD arg3) override final
    {
        return getFinal().bltBatch(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT BltFast(DWORD arg1, DWORD arg2, Interface * arg3, LPRECT arg4, DWORD arg5) override final
    {
        return getFinal().bltFast(arg1, arg2, arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT DeleteAttachedSurface(DWORD arg1, Interface * arg2) override final
    {
        return getFinal().deleteAttachedSurface(arg1, arg2);
    }

    virtual __stdcall HRESULT EnumAttachedSurfaces(LPVOID arg1, EnumSurfacesCallback arg2) override final
    {
        return getFinal().enumAttachedSurfaces(arg1, arg2);
    }

    virtual __stdcall HRESULT EnumOverlayZOrders(DWORD arg1, LPVOID arg2, EnumSurfacesCallback arg3) override final
    {
        return getFinal().enumOverlayZOrders(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT Flip(Interface * arg1, DWORD arg2) override final
    {
        return getFinal().flip(arg1, arg2);
    }

    virtual __stdcall HRESULT GetAttachedSurface(SurfaceCapabilities * arg1, Interface * FAR * arg2) override final
    {
        return getFinal().getAttachedSurface(arg1, arg2);
    }

    virtual __stdcall HRESULT GetBltStatus(DWORD arg) override final
    {
        return getFinal().getBltStatus(arg);
    }

    virtual __stdcall HRESULT GetCaps(SurfaceCapabilities * arg) override final
    {
        return getFinal().getCaps(arg);
    }

    virtual __stdcall HRESULT GetClipper(LPDIRECTDRAWCLIPPER FAR * arg) override final
    {
        return getFinal().getClipper(arg);
    }

    virtual __stdcall HRESULT GetColorKey(DWORD arg1, LPDDCOLORKEY arg2) override final
    {
        return getFinal().getColorKey(arg1, arg2);
    }

    virtual __stdcall HRESULT GetDC(HDC FAR * arg) override final
    {
        return getFinal().getDC(arg);
    }

    virtual __stdcall HRESULT GetFlipStatus(DWORD arg) override final
    {
        return getFinal().getFlipStatus(arg);
    }

    virtual __stdcall HRESULT GetOverlayPosition(LPLONG arg1, LPLONG arg2) override final
    {
        return getFinal().getOverlayPosition(arg1, arg2);
    }

    virtual __stdcall HRESULT GetPalette(LPDIRECTDRAWPALETTE FAR * arg) override final
    {
        return getFinal().getPalette(arg);
    }

    virtual __stdcall HRESULT GetPixelFormat(LPDDPIXELFORMAT arg) override final
    {
        return getFinal().getPixelFormat(arg);
    }

    virtual __stdcall HRESULT GetSurfaceDesc(SurfaceDescription * arg) override final
    {
        return getFinal().getSurfaceDesc(arg);
    }

    virtual __stdcall HRESULT Initialize(LPDIRECTDRAW arg1, SurfaceDescription * arg2) override final
    {
        return getFinal().initialize(arg1, arg2);
    }

    virtual __stdcall HRESULT IsLost() override final
    {
        return getFinal().isLost();
    }

    virtual __stdcall HRESULT Lock(LPRECT arg1, SurfaceDescription * arg2, DWORD arg3, HANDLE arg4) override final
    {
        return getFinal().lock(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT ReleaseDC(HDC arg) override final
    {
        return getFinal().releaseDC(arg);
    }

    virtual __stdcall HRESULT Restore() override final
    {
        return getFinal().restore();
    }

    virtual __stdcall HRESULT SetClipper(LPDIRECTDRAWCLIPPER arg) override final
    {
        return getFinal().setClipper(arg);
    }

    virtual __stdcall HRESULT SetColorKey(DWORD arg1, LPDDCOLORKEY arg2) override final
    {
        return getFinal().setColorKey(arg1, arg2);
    }

    virtual __stdcall HRESULT SetOverlayPosition(LONG arg1, LONG arg2) override final
    {
        return getFinal().setOverlayPosition(arg1, arg2);
    }

    virtual __stdcall HRESULT SetPalette(LPDIRECTDRAWPALETTE arg) override final
    {
        return getFinal().setPalette(arg);
    }

    virtual __stdcall HRESULT Unlock(LPRECT arg) override final
    {
        return getFinal().unlock(arg);
    }

    virtual __stdcall HRESULT UpdateOverlay(LPRECT arg1, Interface * arg2, LPRECT arg3, DWORD arg4, LPDDOVERLAYFX arg5) override final
    {
        return getFinal().updateOverlay(arg1, arg2, arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT UpdateOverlayDisplay(DWORD arg) override final
    {
        return getFinal().updateOverlayDisplay(arg);
    }

    virtual __stdcall HRESULT UpdateOverlayZOrder(DWORD arg1, Interface * arg2) override final
    {
        return getFinal().updateOverlayZOrder(arg1, arg2);
    }

    virtual __stdcall HRESULT GetDDInterface(LPVOID FAR * arg) override final
    {
        return getFinal().getDDInterface(arg);
    }

    virtual __stdcall HRESULT PageLock(DWORD arg) override final
    {
        return getFinal().pageLock(arg);
    }

    virtual __stdcall HRESULT PageUnlock(DWORD arg) override final
    {
        return getFinal().pageUnlock(arg);
    }

    virtual __stdcall HRESULT SetSurfaceDesc(SurfaceDescription * arg1, DWORD arg2) override final
    {
        return getFinal().setSurfaceDesc(arg1, arg2);
    }

    virtual __stdcall HRESULT SetPrivateData(REFGUID arg1, LPVOID arg2, DWORD arg3, DWORD arg4) override final
    {
        return getFinal().setPrivateData(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT GetPrivateData(REFGUID arg1, LPVOID arg2, LPDWORD arg3) override final
    {
        return getFinal().getPrivateData(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT FreePrivateData(REFGUID arg) override final
    {
        return getFinal().freePrivateData(arg);
    }

    virtual __stdcall HRESULT GetUniquenessValue(LPDWORD arg) override final
    {
        return getFinal().getUniquenessValue(arg);
    }

    virtual __stdcall HRESULT ChangeUniquenessValue() override final
    {
        return getFinal().changeUniquenessValue();
    }

    virtual __stdcall HRESULT SetPriority(DWORD arg) override final
    {
        return getFinal().setPriority(arg);
    }

    virtual __stdcall HRESULT GetPriority(LPDWORD arg) override final
    {
        return getFinal().getPriority(arg);
    }

    virtual __stdcall HRESULT SetLOD(DWORD arg) override final
    {
        return getFinal().setLOD(arg);
    }

    virtual __stdcall HRESULT GetLOD(LPDWORD arg) override final
    {
        return getFinal().getLOD(arg);
    }
};

}
