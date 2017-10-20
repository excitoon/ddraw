#pragma once

#include <windows.h>

#include "Unknown.h"


template <typename SurfaceHolder>
class DirectDrawSurface7 : public Unknown<DirectDrawSurface7<SurfaceHolder>>
{
    IDirectDrawSurface7 * underlying;
    SurfaceHolder & surface_holder;

public:
    DirectDrawSurface7(SurfaceHolder & surface_holder, IDirectDrawSurface7 * underlying):
        Unknown<DirectDrawSurface7>(underlying),
        underlying(underlying),
        surface_holder(surface_holder)
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
        return underlying->AddAttachedSurface(surface_holder.getOriginalSurface(arg));
    }

    virtual __stdcall HRESULT AddOverlayDirtyRect(LPRECT arg)
    {
        return underlying->AddOverlayDirtyRect(arg);
    }

    virtual __stdcall HRESULT Blt(LPRECT arg1, LPDIRECTDRAWSURFACE7 arg2, LPRECT arg3, DWORD arg4, LPDDBLTFX arg5)
    {
        return underlying->Blt(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT BltBatch(LPDDBLTBATCH arg1, DWORD arg2, DWORD arg3)
    {
        return underlying->BltBatch(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT BltFast(DWORD arg1, DWORD arg2, LPDIRECTDRAWSURFACE7 arg3, LPRECT arg4, DWORD arg5)
    {
        return underlying->BltFast(arg1, arg2, surface_holder.getOriginalSurface(arg3), arg4, arg5);
    }

    virtual __stdcall HRESULT DeleteAttachedSurface(DWORD arg1, LPDIRECTDRAWSURFACE7 arg2)
    {
        return underlying->DeleteAttachedSurface(arg1, surface_holder.getOriginalSurface(arg2));
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
        return context.lpEnumSurfacesCallback(context.direct_draw_surface7.getSurfaceHolder().getWrappedSurface(lpDDSurface), lpDDSurfaceDesc, context.lpContext);
    }

    virtual __stdcall HRESULT EnumAttachedSurfaces(LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback)
    {
        WrapperContext wrapper_context{ lpContext, lpEnumSurfacesCallback, 0, *this };
        return underlying->EnumAttachedSurfaces(static_cast<LPVOID>(&wrapper_context), EnumSurfacesCallback7);
    }

    virtual __stdcall HRESULT EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpfnCallback)
    {
        WrapperContext wrapper_context{ lpContext, lpfnCallback, dwFlags, *this };
        return underlying->EnumOverlayZOrders(dwFlags, static_cast<LPVOID>(&wrapper_context), EnumSurfacesCallback7);
    }

    virtual __stdcall HRESULT Flip(LPDIRECTDRAWSURFACE7 arg1, DWORD arg2)
    {
        return underlying->Flip(surface_holder.getOriginalSurface(arg1), arg2);
    }

    virtual __stdcall HRESULT GetAttachedSurface(LPDDSCAPS2 lpDDSCaps, LPDIRECTDRAWSURFACE7 FAR * lplpDDAttachedSurface)
    {
        LPDIRECTDRAWSURFACE7 lpDDAttachedSurface = nullptr;
        HRESULT result = underlying->GetAttachedSurface(lpDDSCaps, &lpDDAttachedSurface);
        *lplpDDAttachedSurface = surface_holder.getWrappedSurface(lpDDAttachedSurface);
        return result;
    }

    virtual __stdcall HRESULT GetBltStatus(DWORD arg)
    {
        return underlying->GetBltStatus(arg);
    }

    virtual __stdcall HRESULT GetCaps(LPDDSCAPS2 arg)
    {
        return underlying->GetCaps(arg);
    }

    virtual __stdcall HRESULT GetClipper(LPDIRECTDRAWCLIPPER FAR * arg)
    {
        return underlying->GetClipper(arg);
    }

    virtual __stdcall HRESULT GetColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        return underlying->GetColorKey(arg1, arg2);
    }

    virtual __stdcall HRESULT GetDC(HDC FAR * arg)
    {
        return underlying->GetDC(arg);
    }

    virtual __stdcall HRESULT GetFlipStatus(DWORD arg)
    {
        return underlying->GetFlipStatus(arg);
    }

    virtual __stdcall HRESULT GetOverlayPosition(LPLONG arg1, LPLONG arg2)
    {
        return underlying->GetOverlayPosition(arg1, arg2);
    }

    virtual __stdcall HRESULT GetPalette(LPDIRECTDRAWPALETTE FAR * arg)
    {
        return underlying->GetPalette(arg);
    }

    virtual __stdcall HRESULT GetPixelFormat(LPDDPIXELFORMAT arg)
    {
        return underlying->GetPixelFormat(arg);
    }

    virtual __stdcall HRESULT GetSurfaceDesc(LPDDSURFACEDESC2 arg)
    {
        return underlying->GetSurfaceDesc(arg);
    }

    virtual __stdcall HRESULT Initialize(LPDIRECTDRAW arg1, LPDDSURFACEDESC2 arg2)
    {
        return underlying->Initialize(arg1, arg2);
    }

    virtual __stdcall HRESULT IsLost()
    {
        return underlying->IsLost();
    }

    virtual __stdcall HRESULT Lock(LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
    {
        return underlying->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent);
    }

    virtual __stdcall HRESULT ReleaseDC(HDC arg)
    {
        return underlying->ReleaseDC(arg);
    }

    virtual __stdcall HRESULT Restore()
    {
        return underlying->Restore();
    }

    virtual __stdcall HRESULT SetClipper(LPDIRECTDRAWCLIPPER arg)
    {
        return underlying->SetClipper(arg);
    }

    virtual __stdcall HRESULT SetColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        return underlying->SetColorKey(arg1, arg2);
    }

    virtual __stdcall HRESULT SetOverlayPosition(LONG arg1, LONG arg2)
    {
        return underlying->SetOverlayPosition(arg1, arg2);
    }

    virtual __stdcall HRESULT SetPalette(LPDIRECTDRAWPALETTE arg)
    {
        return underlying->SetPalette(arg);
    }

    virtual __stdcall HRESULT Unlock(LPRECT lpRect)
    {
        return underlying->Unlock(lpRect);
    }

    virtual __stdcall HRESULT UpdateOverlay(LPRECT arg1, LPDIRECTDRAWSURFACE7 arg2, LPRECT arg3, DWORD arg4, LPDDOVERLAYFX arg5)
    {
        return underlying->UpdateOverlay(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT UpdateOverlayDisplay(DWORD arg)
    {
        return underlying->UpdateOverlayDisplay(arg);
    }

    virtual __stdcall HRESULT UpdateOverlayZOrder(DWORD arg1, LPDIRECTDRAWSURFACE7 arg2)
    {
        return underlying->UpdateOverlayZOrder(arg1, surface_holder.getOriginalSurface(arg2));
    }

    virtual __stdcall HRESULT GetDDInterface(LPVOID FAR * lplpDD)
    {
        LPVOID lpDD;
        HRESULT result = underlying->GetDDInterface(&lpDD);
        *lplpDD = static_cast<LPVOID>(&surface_holder);
        return result;
    }

    virtual __stdcall HRESULT PageLock(DWORD arg)
    {
        return underlying->PageLock(arg);
    }

    virtual __stdcall HRESULT PageUnlock(DWORD arg)
    {
        return underlying->PageUnlock(arg);
    }

    virtual __stdcall HRESULT SetSurfaceDesc(LPDDSURFACEDESC2 arg1, DWORD arg2)
    {
        return underlying->SetSurfaceDesc(arg1, arg2);
    }

    virtual __stdcall HRESULT SetPrivateData(REFGUID arg1, LPVOID arg2, DWORD arg3, DWORD arg4)
    {
        return underlying->SetPrivateData(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT GetPrivateData(REFGUID arg1, LPVOID arg2, LPDWORD arg3)
    {
        return underlying->GetPrivateData(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT FreePrivateData(REFGUID arg)
    {
        return underlying->FreePrivateData(arg);
    }

    virtual __stdcall HRESULT GetUniquenessValue(LPDWORD arg)
    {
        return underlying->GetUniquenessValue(arg);
    }

    virtual __stdcall HRESULT ChangeUniquenessValue()
    {
        return underlying->ChangeUniquenessValue();
    }

    virtual __stdcall HRESULT SetPriority(DWORD arg)
    {
        return underlying->SetPriority(arg);
    }

    virtual __stdcall HRESULT GetPriority(LPDWORD arg)
    {
        return underlying->GetPriority(arg);
    }

    virtual __stdcall HRESULT SetLOD(DWORD arg)
    {
        return underlying->SetLOD(arg);
    }

    virtual __stdcall HRESULT GetLOD(LPDWORD arg)
    {
        return underlying->GetLOD(arg);
    }
};
