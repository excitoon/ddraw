#pragma once

#include <windows.h>
#include <unwindows.h>


namespace interface
{

class Unknown
{
public:
    virtual __stdcall HRESULT QueryInterface(REFIID riid, LPVOID * ppvObject) = 0;
    virtual __stdcall ULONG AddRef() = 0;
    virtual __stdcall ULONG Release() = 0;
};

}
