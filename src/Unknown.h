#pragma once

#include <windows.h>


template <typename Final>
class Unknown
{
    size_t ref_count = 1;
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

    virtual __attribute__((__stdcall__)) void QueryInterface(REFIID riid, void ** ppvObject)
    {
        underlying->QueryInterface(riid, ppvObject);
    }

    virtual __attribute__((__stdcall__)) size_t AddRef()
    {
        return ++ref_count;
    }

    virtual __attribute__((__stdcall__)) size_t Release()
    {
        --ref_count;
        if (ref_count == 0)
        {
            delete getFinal();
        }
        return ref_count;
    }
};
