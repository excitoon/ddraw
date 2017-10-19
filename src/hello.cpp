#include <windows.h>

int main()
{
    typedef int (__attribute__((__stdcall__)) * pfunc)();
    HMODULE hdll;
    pfunc hello;
    hdll = LoadLibrary("ddraw.dll");
    hello = static_cast<pfunc>(GetProcAddress(hdll, "hello"));
    int result = hello ? hello() : 0;
    FreeLibrary(hdll);
    return result == 42 ? 0 : 1;
}
