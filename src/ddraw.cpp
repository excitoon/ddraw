#include <windows.h>
#include <ddraw.h>

#include "DirectDraw.h"


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DirectDraw::create();
            break;

        case DLL_PROCESS_DETACH:
            DirectDraw::destroy();
            break;
    }

    return TRUE;
}

extern "C" __declspec(dllexport) int hello()
{
    return 42;
}

extern "C" __declspec(dllexport) HRESULT WINAPI DirectDrawCreate(GUID FAR * lpGUID, LPDIRECTDRAW FAR * lplpDD, IUnknown FAR * pUnkOuter)
{
    return DirectDraw::DirectDrawCreate(lpGUID, lplpDD, pUnkOuter);
}

extern "C" __declspec(dllexport) HRESULT WINAPI DirectDrawEnumerate(LPDDENUMCALLBACK lpCallback, LPVOID lpContext)
{
    return DirectDraw::DirectDrawEnumerate(lpCallback, lpContext);
}
