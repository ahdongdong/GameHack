#pragma once
#include <Windows.h>

class CProcessHelper
{
public:
    CProcessHelper();
    virtual ~CProcessHelper(void);
public:
    BOOL Open(DWORD dwID, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS);
    BOOL Close();
    BOOL Terminal();
    BOOL Suspend();
    BOOL Resume();
	HANDLE CreateThread(LPTHREAD_START_ROUTINE lpAddr,LPVOID dwThreadParam);
    LPVOID VirtualAllocEx(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
    BOOL VirtualFreeEx(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
    BOOL ReadMemory(LPCVOID lpAddr, LPVOID lpBuf, DWORD dwReadSize);
    BOOL WriteMemory(LPVOID lpAddr, LPCVOID lpBuf, DWORD dwWriteSize);
    DWORD GetPID()const;
    HANDLE GetModuleHandle();
    DWORD GetMainThreadID();
private:
    DWORD m_dwID;
    HANDLE m_hProcess;
};

