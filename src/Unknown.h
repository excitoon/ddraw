#pragma once

#include <windows.h>

#include "Scheduler.h"


template <typename Final>
class Unknown
{
    Scheduler & scheduler;
    IUnknown * underlying;

public:
    Unknown(Scheduler & scheduler, IUnknown * underlying):
        scheduler(scheduler),
        underlying(underlying)
    {
    }

    Final * getFinal()
    {
        return static_cast<Final *>(this);
    }

    virtual __stdcall HRESULT QueryInterface(REFIID riid, void ** ppvObject)
    {
        return scheduler.makeTask<HRESULT>([&]() { return underlying->QueryInterface(riid, ppvObject); });
    }

    virtual __stdcall ULONG AddRef()
    {
        return scheduler.makeTask<ULONG>([&]() { return underlying->AddRef(); });
    }

    virtual __stdcall ULONG Release()
    {
        ULONG ref_count = scheduler.makeTask<ULONG>([&]() { return underlying->Release(); });
        if (ref_count == 0)
        {
            delete getFinal();
        }
        return ref_count;
    }
};
