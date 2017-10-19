#pragma once

#include <string>
#include <stdexcept>

#include <windows.h>
#include <ddraw.h>

#include "DirectDraw7.h"


class DirectDraw
{
    static HMODULE hdll;
    static HRESULT WINAPI (__attribute__((__stdcall__)) * OriginalDirectDrawCreate)(GUID FAR * lpGUID, LPDIRECTDRAW FAR * lplpDD, IUnknown FAR * pUnkOuter);
    static HRESULT WINAPI (__attribute__((__stdcall__)) * OriginalDirectDrawEnumerate)(LPDDENUMCALLBACK lpCallback, LPVOID lpContext);

    static std::string getSystemDirectory()
    {
        char buf[8192] = { 0 };
        GetSystemDirectory(buf, sizeof(buf));
        return buf;
    }

public:
    static void create()
    {
        std::string filename = getSystemDirectory() + "\\ddraw.dll";
        hdll = LoadLibrary(filename.data());
        if (hdll == NULL)
        {
            throw std::runtime_error("Can't load library.");
        }
        OriginalDirectDrawCreate = reinterpret_cast<decltype(OriginalDirectDrawCreate)>(GetProcAddress(hdll, "DirectDrawCreate"));
        if (OriginalDirectDrawCreate == NULL)
        {
            throw std::runtime_error("Can't get address of DirectDrawCreate.");
        }
        OriginalDirectDrawEnumerate = reinterpret_cast<decltype(OriginalDirectDrawEnumerate)>(GetProcAddress(hdll, "DirectDrawEnumerateA"));
        if (OriginalDirectDrawEnumerate == NULL)
        {
            throw std::runtime_error("Can't get address of DirectDrawEnumerate.");
        }
    }

    static void destroy()
    {
        FreeLibrary(hdll);
    }

    static HRESULT WINAPI DirectDrawCreate(GUID FAR * lpGUID, LPDIRECTDRAW FAR * lplpDD, IUnknown FAR * pUnkOuter)
    {
        LPDIRECTDRAW lpDD;
        HRESULT result = OriginalDirectDrawCreate(lpGUID, &lpDD, pUnkOuter);
        if (lpDD != nullptr)
        {
            *lplpDD = reinterpret_cast<LPDIRECTDRAW>(new DirectDraw7(reinterpret_cast<IDirectDraw7 *>(lpDD)));
        }
        else
        {
            *lplpDD = lpDD;
        }
        return result;
    }

    static HRESULT WINAPI DirectDrawEnumerate(LPDDENUMCALLBACK lpCallback, LPVOID lpContext)
    {
        return OriginalDirectDrawEnumerate(lpCallback, lpContext);
    }
};

HMODULE DirectDraw::hdll;
HRESULT WINAPI (__attribute__((__stdcall__)) * DirectDraw::OriginalDirectDrawCreate)(GUID FAR * lpGUID, LPDIRECTDRAW FAR * lplpDD, IUnknown FAR * pUnkOuter);
HRESULT WINAPI (__attribute__((__stdcall__)) * DirectDraw::OriginalDirectDrawEnumerate)(LPDDENUMCALLBACK lpCallback, LPVOID lpContext);
