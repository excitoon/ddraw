#pragma once

#include <vector>
#include <windows.h>

#include "Logger.h"
#include "Unknown.h"


template <typename SurfaceHolder>
class DirectDrawSurface7 : public Unknown<DirectDrawSurface7<SurfaceHolder>>
{
    IDirectDrawSurface7 * underlying;
    SurfaceHolder & surface_holder;

    bool emulate_16_bits_per_pixel;
    bool is_primary;

    /// Lock() data.
    void * memory = nullptr;
    std::vector<unsigned char> buffer;
    unsigned width;
    unsigned height;

	Logger log = Logger(Logger::Level::Trace, "DirectDrawSurface7");

public:
    DirectDrawSurface7(SurfaceHolder & surface_holder, IDirectDrawSurface7 * underlying, bool emulate_16_bits_per_pixel, bool is_primary):
        Unknown<DirectDrawSurface7>(underlying),
        underlying(underlying),
        surface_holder(surface_holder),
        emulate_16_bits_per_pixel(emulate_16_bits_per_pixel),
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
        return underlying->AddAttachedSurface(surface_holder.getOriginalSurface(arg));
    }

    virtual __stdcall HRESULT AddOverlayDirtyRect(LPRECT arg)
    {
        log() << "AddOverlayDirtyRect.";
        return underlying->AddOverlayDirtyRect(arg);
    }

    virtual __stdcall HRESULT Blt(LPRECT arg1, LPDIRECTDRAWSURFACE7 arg2, LPRECT arg3, DWORD arg4, LPDDBLTFX arg5)
    {
        log() << "Blt.";
        return underlying->Blt(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT BltBatch(LPDDBLTBATCH arg1, DWORD arg2, DWORD arg3)
    {
        log() << "BltBatch.";
        return underlying->BltBatch(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT BltFast(DWORD arg1, DWORD arg2, LPDIRECTDRAWSURFACE7 arg3, LPRECT arg4, DWORD arg5)
    {
        log() << "BltFast.";
        return underlying->BltFast(arg1, arg2, surface_holder.getOriginalSurface(arg3), arg4, arg5);
    }

    virtual __stdcall HRESULT DeleteAttachedSurface(DWORD arg1, LPDIRECTDRAWSURFACE7 arg2)
    {
        log() << "DeleteAttachedSurface.";
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
        context.direct_draw_surface7.log() << "EnumSurfacesCallback7.";
        return context.lpEnumSurfacesCallback(context.direct_draw_surface7.getSurfaceHolder().getWrappedSurface(lpDDSurface), lpDDSurfaceDesc, context.lpContext);
    }

    virtual __stdcall HRESULT EnumAttachedSurfaces(LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback)
    {
        log() << "EnumAttachedSurfaces.";
        WrapperContext wrapper_context{ lpContext, lpEnumSurfacesCallback, 0, *this };
        return underlying->EnumAttachedSurfaces(static_cast<LPVOID>(&wrapper_context), EnumSurfacesCallback7);
    }

    virtual __stdcall HRESULT EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpfnCallback)
    {
        log() << "EnumOverlayZOrders.";
        WrapperContext wrapper_context{ lpContext, lpfnCallback, dwFlags, *this };
        return underlying->EnumOverlayZOrders(dwFlags, static_cast<LPVOID>(&wrapper_context), EnumSurfacesCallback7);
    }

    virtual __stdcall HRESULT Flip(LPDIRECTDRAWSURFACE7 arg1, DWORD arg2)
    {
        log() << "Flip.";
        return underlying->Flip(surface_holder.getOriginalSurface(arg1), arg2);
    }

    virtual __stdcall HRESULT GetAttachedSurface(LPDDSCAPS2 lpDDSCaps, LPDIRECTDRAWSURFACE7 FAR * lplpDDAttachedSurface)
    {
        log() << "GetAttachedSurface.";
        LPDIRECTDRAWSURFACE7 lpDDAttachedSurface = nullptr;
        HRESULT result = underlying->GetAttachedSurface(lpDDSCaps, &lpDDAttachedSurface);
        *lplpDDAttachedSurface = surface_holder.getWrappedSurface(lpDDAttachedSurface);
        return result;
    }

    virtual __stdcall HRESULT GetBltStatus(DWORD arg)
    {
        log() << "GetBltStatus.";
        return underlying->GetBltStatus(arg);
    }

    virtual __stdcall HRESULT GetCaps(LPDDSCAPS2 arg)
    {
        log() << "GetCaps.";
        return underlying->GetCaps(arg);
    }

    virtual __stdcall HRESULT GetClipper(LPDIRECTDRAWCLIPPER FAR * arg)
    {
        log() << "GetClipper.";
        return underlying->GetClipper(arg);
    }

    virtual __stdcall HRESULT GetColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        log() << "GetColorKey.";
        return underlying->GetColorKey(arg1, arg2);
    }

    virtual __stdcall HRESULT GetDC(HDC FAR * arg)
    {
        log() << "GetDC.";
        return underlying->GetDC(arg);
    }

    virtual __stdcall HRESULT GetFlipStatus(DWORD arg)
    {
        log() << "GetFlipStatus.";
        return underlying->GetFlipStatus(arg);
    }

    virtual __stdcall HRESULT GetOverlayPosition(LPLONG arg1, LPLONG arg2)
    {
        log() << "GetOverlayPosition.";
        return underlying->GetOverlayPosition(arg1, arg2);
    }

    virtual __stdcall HRESULT GetPalette(LPDIRECTDRAWPALETTE FAR * arg)
    {
        log() << "GetPalette.";
        return underlying->GetPalette(arg);
    }

    virtual __stdcall HRESULT GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
    {
        HRESULT result = underlying->GetPixelFormat(lpDDPixelFormat);
        log() << "GetPixelFormat(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec << ")"
            << " -> flags=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwFlags << std::dec
            << ", bpp=" << lpDDPixelFormat->dwRGBBitCount
            << ", r=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwRBitMask << std::dec
            << ", g=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwGBitMask << std::dec
            << ", b=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwBBitMask << std::dec
            << ", a=" << std::hex << std::setfill('0') << std::setw(8) << lpDDPixelFormat->dwRGBAlphaBitMask << std::dec << ".";
        if (emulate_16_bits_per_pixel)
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
        return underlying->GetSurfaceDesc(arg);
    }

    virtual __stdcall HRESULT Initialize(LPDIRECTDRAW arg1, LPDDSURFACEDESC2 arg2)
    {
        log() << "Initialize.";
        return underlying->Initialize(arg1, arg2);
    }

    virtual __stdcall HRESULT IsLost()
    {
        log() << "IsLost.";
        return underlying->IsLost();
    }

private:
    inline unsigned short packHiColor(unsigned color)
    {
        unsigned char r = (color >> 16) & 0x1F;
        unsigned char g = (color >> 8) & 0x3F;
        unsigned char b = color & 0x1F;
        return (r << (5+6)) | (g << 5) | b;
    }

public:
    virtual __stdcall HRESULT Lock(LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
    {
        HRESULT result = underlying->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent);
        log(Logger::Level::Trace) << "Lock(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", rect=" << std::hex << std::setfill('0') << std::setw(8) << lpDestRect << std::dec
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << dwFlags << std::dec << ")"
            << " -> width=" << lpDDSurfaceDesc->dwWidth << ", height=" << lpDDSurfaceDesc->dwHeight
            << ", flags=" << std::hex << std::setfill('0') << std::setw(8) << lpDDSurfaceDesc->dwFlags << std::dec << ".";
        if (result == DD_OK)
        {
            /// Will hold one region per surface.
            if (lpDDSurfaceDesc->dwFlags & DDSD_WIDTH && lpDDSurfaceDesc->dwFlags & DDSD_HEIGHT)
            {
                memory = lpDDSurfaceDesc->lpSurface;
                height = lpDDSurfaceDesc->dwHeight;
                width = lpDDSurfaceDesc->dwWidth;
                if (emulate_16_bits_per_pixel && is_primary)
                {
                    buffer.resize(height*width*4);
                    for (unsigned y = 0; y < height; ++y)
                    {
                        unsigned * input = reinterpret_cast<unsigned *>(reinterpret_cast<unsigned char *>(memory) + y * width * 4);
                        unsigned short * output = reinterpret_cast<unsigned short *>(buffer.data() + y * width * 4);
                        for (unsigned x = 0; x < width; ++x)
                        {
                            *output++ = packHiColor(*input++);                     
                        }
                    }

                    lpDDSurfaceDesc->lpSurface = buffer.data();
                }
            }
        }
        return result;
    }

    virtual __stdcall HRESULT ReleaseDC(HDC arg)
    {
        log() << "ReleaseDC.";
        return underlying->ReleaseDC(arg);
    }

    virtual __stdcall HRESULT Restore()
    {
        log() << "Restore.";
        return underlying->Restore();
    }

    virtual __stdcall HRESULT SetClipper(LPDIRECTDRAWCLIPPER arg)
    {
        log() << "SetClipper.";
        return underlying->SetClipper(arg);
    }

    virtual __stdcall HRESULT SetColorKey(DWORD arg1, LPDDCOLORKEY arg2)
    {
        log() << "SetColorKey.";
        return underlying->SetColorKey(arg1, arg2);
    }

    virtual __stdcall HRESULT SetOverlayPosition(LONG arg1, LONG arg2)
    {
        log() << "SetOverlayPosition.";
        return underlying->SetOverlayPosition(arg1, arg2);
    }

    virtual __stdcall HRESULT SetPalette(LPDIRECTDRAWPALETTE arg)
    {
        log() << "SetPalette.";
        return underlying->SetPalette(arg);
    }

private:
    inline unsigned unpackHiColor(unsigned short color)
    {
        unsigned char r = (color & 0xF800) >> (5+6-3);
        unsigned char g = (color & 0x07E0) >> (6-3);
        unsigned char b = (color & 0x001F) << (3);
        return (r << 16) | (g << 8) | b;
    }

public:
    virtual __stdcall HRESULT Unlock(LPRECT lpRect)
    {
        if (emulate_16_bits_per_pixel && is_primary)
        {
            for (unsigned y = 0; y < height; ++y)
            {
                unsigned short * input = reinterpret_cast<unsigned short *>(buffer.data() + y * width * 4);
                unsigned * output = reinterpret_cast<unsigned *>(reinterpret_cast<unsigned char *>(memory) + y * width * 4);
                for (unsigned x = 0; x < width; ++x)
                {
                    *output++ = unpackHiColor(*input++);
                }
            }
        }
        log(Logger::Level::Trace) << "Unlock(this=" << std::hex << std::setfill('0') << std::setw(8) << this << std::dec
            << ", rect=" << std::hex << std::setfill('0') << std::setw(8) << lpRect << std::dec << ").";
        return underlying->Unlock(lpRect);
    }

    virtual __stdcall HRESULT UpdateOverlay(LPRECT arg1, LPDIRECTDRAWSURFACE7 arg2, LPRECT arg3, DWORD arg4, LPDDOVERLAYFX arg5)
    {
        log() << "UpdateOverlay.";
        return underlying->UpdateOverlay(arg1, surface_holder.getOriginalSurface(arg2), arg3, arg4, arg5);
    }

    virtual __stdcall HRESULT UpdateOverlayDisplay(DWORD arg)
    {
        log() << "UpdateOverlayDisplay.";
        return underlying->UpdateOverlayDisplay(arg);
    }

    virtual __stdcall HRESULT UpdateOverlayZOrder(DWORD arg1, LPDIRECTDRAWSURFACE7 arg2)
    {
        log() << "UpdateOverlayZOrder.";
        return underlying->UpdateOverlayZOrder(arg1, surface_holder.getOriginalSurface(arg2));
    }

    virtual __stdcall HRESULT GetDDInterface(LPVOID FAR * lplpDD)
    {
        log() << "GetDDInterface.";
        LPVOID lpDD;
        HRESULT result = underlying->GetDDInterface(&lpDD);
        *lplpDD = static_cast<LPVOID>(&surface_holder);
        return result;
    }

    virtual __stdcall HRESULT PageLock(DWORD arg)
    {
        log() << "PageLock.";
        return underlying->PageLock(arg);
    }

    virtual __stdcall HRESULT PageUnlock(DWORD arg)
    {
        log() << "PageUnlock.";
        return underlying->PageUnlock(arg);
    }

    virtual __stdcall HRESULT SetSurfaceDesc(LPDDSURFACEDESC2 arg1, DWORD arg2)
    {
        log() << "SetSurfaceDesc.";
        return underlying->SetSurfaceDesc(arg1, arg2);
    }

    virtual __stdcall HRESULT SetPrivateData(REFGUID arg1, LPVOID arg2, DWORD arg3, DWORD arg4)
    {
        log() << "SetPrivateData.";
        return underlying->SetPrivateData(arg1, arg2, arg3, arg4);
    }

    virtual __stdcall HRESULT GetPrivateData(REFGUID arg1, LPVOID arg2, LPDWORD arg3)
    {
        log() << "GetPrivateData.";
        return underlying->GetPrivateData(arg1, arg2, arg3);
    }

    virtual __stdcall HRESULT FreePrivateData(REFGUID arg)
    {
        log() << "FreePrivateData.";
        return underlying->FreePrivateData(arg);
    }

    virtual __stdcall HRESULT GetUniquenessValue(LPDWORD arg)
    {
        log() << "GetUniquenessValue.";
        return underlying->GetUniquenessValue(arg);
    }

    virtual __stdcall HRESULT ChangeUniquenessValue()
    {
        log() << "ChangeUniquenessValue.";
        return underlying->ChangeUniquenessValue();
    }

    virtual __stdcall HRESULT SetPriority(DWORD arg)
    {
        log() << "SetPriority.";
        return underlying->SetPriority(arg);
    }

    virtual __stdcall HRESULT GetPriority(LPDWORD arg)
    {
        log() << "GetPriority.";
        return underlying->GetPriority(arg);
    }

    virtual __stdcall HRESULT SetLOD(DWORD arg)
    {
        log() << "SetLOD.";
        return underlying->SetLOD(arg);
    }

    virtual __stdcall HRESULT GetLOD(LPDWORD arg)
    {
        log() << "GetLOD.";
        return underlying->GetLOD(arg);
    }
};
