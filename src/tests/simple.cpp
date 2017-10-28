#include <iostream>

#include <windows.h>
#include <ddraw.h>


int main(int argc, char ** argv)
{
    using DirectDrawCreateType = HRESULT WINAPI __stdcall (*)(GUID FAR * lpGuid, LPDIRECTDRAW * lplpDD, IUnknown FAR * pUnkOuter);
    HMODULE hdll;
    DirectDrawCreateType directDrawCreate;
    hdll = LoadLibrary(argv[1]);
    if (hdll == nullptr)
    {
        std::cout << "LoadLibrary() failed.\n";
        return 6;
    }
    directDrawCreate = reinterpret_cast<DirectDrawCreateType>(GetProcAddress(hdll, "DirectDrawCreate"));
    if (directDrawCreate == nullptr)
    {
        std::cout << "GetProcAddress() failed.\n";
        return 7;
    }
    IDirectDraw * dd;
    HRESULT result = directDrawCreate(nullptr, &dd, nullptr);
    if (result != DD_OK)
    {
        std::cout << "DirectDrawCreate() failed.\n";
        return 1;
    }
    DDSURFACEDESC DDSurfaceDesc = { 0 };
    DDSurfaceDesc.dwSize = sizeof(DDSurfaceDesc);
    DDSurfaceDesc.dwFlags = DDSD_CAPS;
    DDSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    LPDIRECTDRAWSURFACE dds = nullptr;
    HWND hwnd = GetConsoleWindow();
    result = dd->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
    if (result != DD_OK)
    {
        std::cout << "SetCooperativeLevel() failed.\n";
        return 2;
    }
    result = dd->CreateSurface(&DDSurfaceDesc, &dds, nullptr);
    if (result != DD_OK)
    {
        std::cout << "CreateSurface() failed.\n";
        return 3;
    }
    result = dds->Lock(nullptr, &DDSurfaceDesc, 0, nullptr);
    if (result != DD_OK)
    {
        std::cout << "Lock() failed.\n";
        return 4;
    }
    result = dds->Unlock(DDSurfaceDesc.lpSurface);
    if (result != DD_OK)
    {
        std::cout << "Unlock() failed.\n";
        return 5;
    }
    /// Won't call FreeLibrary().
    return 0;
}
