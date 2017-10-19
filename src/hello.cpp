#include <windows.h>

int main()
{
	typedef int (__attribute__((__stdcall__)) * pfunc)();
	HANDLE hdll;
	pfunc hello;
	hdll = LoadLibrary("ddraw.dll");
	hello = static_cast<pfunc>(GetProcAddress(reinterpret_cast<HMODULE>(hdll), "hello"));
	int result = hello ? hello() : 0;
	CloseHandle(hdll);
	return result == 42 ? 0 : 1;
}
