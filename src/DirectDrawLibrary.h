#pragma once

#include <string>
#include <stdexcept>

#include <windows.h>
#include <ddraw.h>

#include <interface/DirectDraw.h>
#include <proxy/DirectDraw.h>
#include <proxy/DirectDrawSurface.h>
#include "DirectDrawFinal.h"
#include "Scheduler.h"
#include "Logger.h"


class DirectDrawLibrary
{
    static HMODULE hdll;
    static HRESULT WINAPI __stdcall (* OriginalDirectDrawCreate)(GUID FAR * lpGuid, LPDIRECTDRAW * lplpDD, IUnknown FAR * pUnkOuter);
    static HRESULT WINAPI __stdcall (* OriginalDirectDrawCreateEx)(GUID FAR * lpGuid, LPVOID * lplpDD, REFIID iid, IUnknown FAR * pUnkOuter);
    static HRESULT WINAPI __stdcall (* OriginalDirectDrawEnumerate)(LPDDENUMCALLBACK lpCallback, LPVOID lpContext);
    static Scheduler * scheduler;

    static Logger log;

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
        if (OriginalDirectDrawCreate == nullptr)
        {
            throw std::runtime_error("Can't get address of DirectDrawCreate.");
        }
        OriginalDirectDrawCreateEx = reinterpret_cast<decltype(OriginalDirectDrawCreateEx)>(GetProcAddress(hdll, "DirectDrawCreateEx"));
        if (OriginalDirectDrawCreateEx == nullptr)
        {
            throw std::runtime_error("Can't get address of DirectDrawCreate.");
        }
        OriginalDirectDrawEnumerate = reinterpret_cast<decltype(OriginalDirectDrawEnumerate)>(GetProcAddress(hdll, "DirectDrawEnumerateA"));
        if (OriginalDirectDrawEnumerate == nullptr)
        {
            throw std::runtime_error("Can't get address of DirectDrawEnumerate.");
        }
        scheduler = new Scheduler();
    }

    static void destroy()
    {
        log() << "Unloading.";
        scheduler->shutDown();
        /// Won't free scheduler to avoid loader deadlock.
        FreeLibrary(hdll);
    }

    static HRESULT WINAPI DirectDrawCreate(GUID FAR * lpGUID, LPDIRECTDRAW FAR * lplpDD, IUnknown FAR * pUnkOuter)
    {
        log() << "DirectDrawCreate.";
        LPDIRECTDRAW lpDD = nullptr;
        HRESULT result = scheduler->makeTask<HRESULT>([&]() { return OriginalDirectDrawCreate(lpGUID, &lpDD, pUnkOuter); });
        if (lpDD != nullptr)
        {
            *lplpDD = reinterpret_cast<IDirectDraw *>(static_cast<interface::DirectDraw *>(
                    new DirectDrawFinal<proxy::DirectDraw, proxy::DirectDrawSurface>(*scheduler, reinterpret_cast<interface::DirectDraw *>(lpDD))));
        }
        else
        {
            *lplpDD = lpDD;
        }
        return result;
    }

    static HRESULT WINAPI DirectDrawEnumerate(LPDDENUMCALLBACK lpCallback, LPVOID lpContext)
    {
        log() << "DirectDrawEnumerate.";
        return scheduler->makeTask<HRESULT>([&]() { return OriginalDirectDrawEnumerate(lpCallback, lpContext); });
    }
};

HMODULE DirectDrawLibrary::hdll;
HRESULT WINAPI __stdcall (* DirectDrawLibrary::OriginalDirectDrawCreate)(GUID FAR * lpGuid, LPDIRECTDRAW * lplpDD, IUnknown FAR * pUnkOuter);
HRESULT WINAPI __stdcall (* DirectDrawLibrary::OriginalDirectDrawCreateEx)(GUID FAR * lpGuid, LPVOID * lplpDD, REFIID iid, IUnknown FAR * pUnkOuter);
HRESULT WINAPI __stdcall (* DirectDrawLibrary::OriginalDirectDrawEnumerate)(LPDDENUMCALLBACK lpCallback, LPVOID lpContext);
Scheduler * DirectDrawLibrary::scheduler;
Logger DirectDrawLibrary::log("DirectDrawLibrary");
