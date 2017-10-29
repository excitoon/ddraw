#include <iostream>

#include <windows.h>

#include <paths.h>


int main(int argc, char ** argv)
{
    using pfunc = int __stdcall (*)();
    HMODULE hdll;
    pfunc hello;
    std::cout << "LoadLibrary.\n" << std::flush;
    hdll = LoadLibrary(argc > 1 ? argv[1] : DDRAW_DEFAULT_PATH);
    if (hdll == nullptr)
    {
        std::cout << "LoadLibrary() failed.\n";
        return 2;
    }
    hello = static_cast<pfunc>(GetProcAddress(hdll, "hello"));
    if (hello == nullptr)
    {
        std::cout << "GetProcAddress() failed.\n";
        return 3;
    }
    std::cout << "Calling hello().\n" << std::flush;
    int result = hello ? hello() : 0;
    std::cout << "FreeLibrary().\n" << std::flush;
    FreeLibrary(hdll);
    return result == 42 ? 0 : 1;
}
