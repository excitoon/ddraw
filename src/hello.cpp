#include <iostream>

#include <windows.h>

int main()
{
    typedef int __stdcall (* pfunc)();
    HMODULE hdll;
    pfunc hello;
    std::cout << "LoadLibrary.\n" << std::flush;
    hdll = LoadLibrary("ddraw.dll");
    hello = static_cast<pfunc>(GetProcAddress(hdll, "hello"));
    std::cout << "Calling hello().\n" << std::flush;
    int result = hello ? hello() : 0;
    std::cout << "FreeLibrary.\n" << std::flush;
    FreeLibrary(hdll);
    return result == 42 ? 0 : 1;
}
