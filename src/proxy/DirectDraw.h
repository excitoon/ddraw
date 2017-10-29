#pragma once

#include <windows.h>

#include <interface/DirectDraw.h>
#include <proxy/Unknown.h>


namespace proxy
{

template <typename Final>
class DirectDraw : public interface::DirectDraw, public Unknown<Final>
{
    Final & getFinal()
    {
        return static_cast<Final &>(*this);
    }

public:
    virtual __stdcall HRESULT Compact() override final
    {
        return getFinal().compact();
    }

    virtual __stdcall HRESULT CreateClipper(DWORD arg1, LPDIRECTDRAWCLIPPER FAR * arg2, IUnknown FAR * arg3) override final
    {
        return getFinal().createClipper(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT CreatePalette(DWORD arg1, LPPALETTEENTRY arg2, LPDIRECTDRAWPALETTE FAR * arg3, IUnknown FAR * arg4) override final
    {
        return getFinal().createPalette(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT CreateSurface(LPDDSURFACEDESC arg1, interface::DirectDrawSurface * FAR * arg2, IUnknown FAR * arg3) override final
    {
        return getFinal().createSurface(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT DuplicateSurface(interface::DirectDrawSurface * arg1, interface::DirectDrawSurface * FAR * arg2) override final
    {
        return getFinal().duplicateSurface(arg1, arg2);
    }

    virtual __stdcall HRESULT EnumDisplayModes(DWORD arg1, LPDDSURFACEDESC arg2, LPVOID arg3, LPDDENUMMODESCALLBACK arg4) override final
    {
        return getFinal().enumDisplayModes(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT EnumSurfaces(DWORD arg1, LPDDSURFACEDESC arg2, LPVOID arg3, EnumSurfacesCallback arg4) override final
    {
        return getFinal().enumSurfaces(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT FlipToGDISurface() override final
    {
        return getFinal().flipToGDISurface();
    }

    virtual __stdcall HRESULT GetCaps(LPDDCAPS arg1, LPDDCAPS arg2) override final
    {
        return getFinal().getCaps(arg1, arg2);
    }

    virtual __stdcall HRESULT GetDisplayMode(LPDDSURFACEDESC arg) override final
    {
        return getFinal().getDisplayMode(arg);
    }

    virtual __stdcall HRESULT GetFourCCCodes(LPDWORD arg1, LPDWORD arg2) override final
    {
        return getFinal().getFourCCCodes(arg1, arg2);
    }

    virtual __stdcall HRESULT GetGDISurface(interface::DirectDrawSurface * FAR * arg) override final
    {
        return getFinal().getGDISurface(arg);
    }

    virtual __stdcall HRESULT GetMonitorFrequency(LPDWORD arg) override final
    {
        return getFinal().getMonitorFrequency(arg);
    }

    virtual __stdcall HRESULT GetScanLine(LPDWORD arg) override final
    {
        return getFinal().getScanLine(arg);
    }

    virtual __stdcall HRESULT GetVerticalBlankStatus(LPBOOL arg) override final
    {
        return getFinal().getVerticalBlankStatus(arg);
    }

    virtual __stdcall HRESULT Initialize(GUID FAR * arg) override final
    {
        return getFinal().initialize(arg);
    }

    virtual __stdcall HRESULT RestoreDisplayMode() override final
    {
        return getFinal().restoreDisplayMode();
    }

    virtual __stdcall HRESULT SetCooperativeLevel(HWND arg1, DWORD arg2) override final
    {
        return getFinal().setCooperativeLevel(arg1, arg2);
    }

    virtual __stdcall HRESULT SetDisplayMode(DWORD arg1, DWORD arg2, DWORD arg3) override final
    {
        return getFinal().setDisplayMode(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT WaitForVerticalBlank(DWORD arg1, HANDLE arg2) override final
    {
        return getFinal().waitForVerticalBlank(arg1, arg2);
    }
};

}
