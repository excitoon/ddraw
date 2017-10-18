#include <string>
#include <iostream>

#include <windows.h>

std::string getSystemDirectory()
{
	char buf[8192] = { 0 };
	GetSystemDirectory(buf, sizeof(buf));
	return buf;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			std::cout << "DLL_PROCESS_ATTACH\n";
			// Code to run when the DLL is loaded
			break;

		case DLL_PROCESS_DETACH:
			std::cout << "DLL_PROCESS_DETACH\n";
			// Code to run when the DLL is freed
			break;

		case DLL_THREAD_ATTACH:
			std::cout << "DLL_THREAD_ATTACH\n";
			// Code to run when a thread is created during the DLL's lifetime
			break;

		case DLL_THREAD_DETACH:
			std::cout << "DLL_THREAD_DETACH\n";
			// Code to run when a thread ends normally.
			break;
	}

	return TRUE;
}

extern "C" __declspec(dllexport) int hello()
{
	return 42;
}
