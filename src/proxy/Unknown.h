#pragma once

#include <windows.h>

#include <interface/Unknown.h>


namespace proxy
{

template <typename Final>
class Unknown : virtual public interface::Unknown
{
    Final & getFinal()
    {
        return static_cast<Final &>(*this);
    }

public:
    virtual __stdcall HRESULT QueryInterface(REFIID riid, LPVOID * ppvObject) override final
    {
        return getFinal().queryInterface(riid, ppvObject);
    }

    virtual __stdcall ULONG AddRef() override final
    {
        return getFinal().addRef();
    }

    virtual __stdcall ULONG Release() override final
    {
        return getFinal().release();
    }
};

}
