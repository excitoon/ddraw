#include <iostream>

#include <windows.h>


int main(int argc, char ** argv)
{
    using pfunc = int __stdcall (*)();
    HMODULE hdll;
    pfunc hello;
    std::cout << "LoadLibrary.\n" << std::flush;
    hdll = LoadLibrary(argv[1]);
    hello = static_cast<pfunc>(GetProcAddress(hdll, "hello"));
    std::cout << "Calling hello().\n" << std::flush;
    int result = hello ? hello() : 0;
    std::cout << "FreeLibrary().\n" << std::flush;
    FreeLibrary(hdll);
    return result == 42 ? 0 : 1;
}
