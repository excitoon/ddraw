#pragma once

#include <windows.h>

#include <interface/Unknown.h>

#include "Logger.h"
#include "Scheduler.h"


template <typename Final, typename Interface>
class UnknownFinal
{
    Scheduler & scheduler;
    interface::Unknown * underlying;
    size_t ref_count = 1;

    Logger log = Logger("Unknown");

    Final & getFinal()
    {
        return static_cast<Final &>(*this);
    }

public:
    UnknownFinal(Scheduler & scheduler, Interface * underlying):
        scheduler(scheduler),
        underlying(reinterpret_cast<interface::Unknown *>(underlying))
        /// This is downside of virtual inheritance, we can't do static_cast<> because this is foreign object.
    {
    }

    HRESULT queryInterface(REFIID riid, void ** ppvObject)
    {
        log() << "QueryInterface.";
        return scheduler.makeTask<HRESULT>([&]() { return underlying->QueryInterface(riid, ppvObject); });
    }

    ULONG addRef()
    {
        log() << "AddRef.";
        return ref_count = scheduler.makeTask<ULONG>([&]() { return underlying->AddRef(); });
    }

    ULONG release()
    {
        log() << "Release() started.";
        interface::Unknown * underlying_copy = this->underlying;
        Scheduler & scheduler_copy = this->scheduler;
        if (ref_count == 1)
        {
            delete &getFinal();
        }
        ULONG ref_count_copy = scheduler_copy.makeTask<ULONG>([&underlying_copy]() { return underlying_copy->Release(); });
        if (ref_count_copy != 0)
        {
            ref_count = ref_count_copy;
        }
        Logger("Unknown")() << "Release() finished.";
        return ref_count_copy;
    }
};
