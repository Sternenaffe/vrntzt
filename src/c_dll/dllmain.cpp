// dllmain.cpp : Defines the entry point for the DLL application.

// TODO: clear up inports
// TODO: change header files to .hpp

#include "include/vrntzt_pch.h"
#include "include/c_dll/framework.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

