#include "stdafx.h"
#include "GameUtility.h"
#include <TlHelp32.h>

HWND GetWindowHandleByPID(DWORD dwProcessID)
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

DWORD GetThreadIDByProcssID(DWORD dwProcessID)
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