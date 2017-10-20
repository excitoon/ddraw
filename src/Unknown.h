#pragma once

#include <windows.h>


template <typename Final>
class Unknown
{
    IUnknown * underlying;

public:
    Unknown(IUnknown * underlying):
        underlying(underlying)
    {
    }

    Final * getFinal()
    {
        return static_cast<Final *>(this);
    }

    virtual __stdcall HRESULT QueryInterface(REFIID riid, void ** ppvObject)
    {
        return underlying->QueryInterface(riid, ppvObject);
    }

    virtual __stdcall ULONG AddRef()
    {
        return underlying->AddRef();
    }

    virtual __stdcall ULONG Release()
    {
        ULONG ref_count = underlying->Release();
        if (ref_count == 0)
        {
            delete getFinal();
        }
        return ref_count;
    }
};
