#include "stdafx.h"
#include "GameUtility.h"
#include <TlHelp32.h>
#include <atlstr.h>

HWND GetHwndFromPID(DWORD dwProcessID)
{
    HWND h = GetTopWindow(0);

    while(h)
    {
        DWORD pid = 0;
        DWORD dwTheardId = GetWindowThreadProcessId(h, &pid);

        if(dwTheardId != 0)
        {
            if(pid == dwProcessID)
            {
                return h;
            }
        }

        h = GetNextWindow(h , GW_HWNDNEXT);
    }

    return NULL;
}

DWORD GetMTIDFromPID(DWORD dwProcessID)
{
    DWORD dwThreadID = 0;
    THREADENTRY32 te32 = {sizeof(te32)};
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);

    if(Thread32First(hThreadSnap, &te32))
    {
        do
        {
            if(dwProcessID == te32.th32OwnerProcessID)
            {
                dwThreadID = te32.th32ThreadID;
                break;
            }
        }
        while(Thread32Next(hThreadSnap, &te32));
    }

    return dwThreadID;
}

BOOL EnablePrivilege(LPCTSTR PrivilegeName)
{
    HANDLE tokenhandle;
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &tokenhandle))
    {
        return FALSE;
    }

    if(!LookupPrivilegeValue(NULL, PrivilegeName, &tp.Privileges[0].Luid))
    {
        CloseHandle(tokenhandle);
        return FALSE;
    }

    if(!AdjustTokenPrivileges(tokenhandle, FALSE, &tp, sizeof(tp), NULL, NULL))
    {
        CloseHandle(tokenhandle);
        return FALSE;
    }

    CloseHandle(tokenhandle);
    return TRUE;
}


DWORD GetPidFromName(LPCTSTR sProName)
{
	DWORD dwPID = -1;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(NULL == hSnap)
	{
		return -1;
	}

	PROCESSENTRY32 tr;
	tr.dwSize = sizeof(PROCESSENTRY32);

	if(Process32First(hSnap, &tr))
	{
		do
		{
			CString sName = tr.szExeFile;

			if(0 == sName.CompareNoCase(sProName))
			{
				dwPID = tr.th32ProcessID;
				break;
			}
		}
		while(Process32Next(hSnap, &tr));
	}

	CloseHandle(hSnap);
	return dwPID;
}

