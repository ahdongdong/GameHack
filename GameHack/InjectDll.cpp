#include "stdafx.h"
#include "InjectDll.h"
#include "GameUtility.h"
#include <tlhelp32.h>
#include <Psapi.h>

CInjectDll::CInjectDll(void)
{
}


CInjectDll::~CInjectDll(void)
{
}

BOOL CInjectDll::RemoteInjectDll(DWORD dwPID, LPCTSTR lpszDllName)
{
    if(!PathFileExists(lpszDllName))
    {
        return FALSE;
    }

    if(!EnablePrivilege(SE_DEBUG_NAME))
    {
        return FALSE;
    }

    HANDLE hPro =::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

    if(NULL == hPro)
    {
        return FALSE;
    }

    LPVOID pszLibFileRemote = NULL;
    HANDLE hRemoteThread = NULL;

    __try
    {
        int cb  = (_tcslen(lpszDllName) + 1) * sizeof(TCHAR);
        pszLibFileRemote = VirtualAllocEx(hPro, NULL, cb, MEM_COMMIT, PAGE_READWRITE);

        if(pszLibFileRemote == NULL)
        {
            return FALSE;
        }

        if(!WriteProcessMemory(hPro, pszLibFileRemote, (PVOID) lpszDllName, cb, NULL))
        {
            return FALSE;
        }

#ifdef _UNICODE
#define Libaray ("LoadLibraryW")
#else
#define Libaray ("LoadLibraryA")
#endif
        PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")), Libaray);

        if(pfnThreadRtn == NULL)
        {
            return FALSE;
        }

        hRemoteThread = ::CreateRemoteThread(hPro, NULL, NULL, pfnThreadRtn, pszLibFileRemote, 0, NULL);

        if(hRemoteThread == NULL)
        {
            return FALSE;
        }

        WaitForSingleObject(hRemoteThread, INFINITE);
        return TRUE;
    }
    __finally
    {
        if(hRemoteThread  != NULL)
        {
            CloseHandle(hRemoteThread);
            hRemoteThread = NULL;
        }

        if(pszLibFileRemote != NULL)
        {
            VirtualFreeEx(hPro, pszLibFileRemote, 0, MEM_RELEASE);
            pszLibFileRemote = NULL;
        }

        if(NULL != hPro)
        {
            ::CloseHandle(hPro);
            hPro = NULL;
        }
    }

    return FALSE;
}


BOOL CInjectDll::RemoteUnInjectDll(DWORD dwPID, LPCTSTR lpszDllName)
{
    if(!EnablePrivilege(SE_DEBUG_NAME))
    {
        return FALSE;
    }

    HANDLE hPro =::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

    if(NULL == hPro)
    {
        return FALSE;
    }

    HANDLE hthSnapshot = NULL;
    HANDLE hThread = NULL;

    __try
    {
        // Grab a new snapshot of the process
        hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

        if(hthSnapshot == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }

        // Get the HMODULE of the desired library
        MODULEENTRY32 me = { sizeof(me) };
        BOOL fFound = FALSE;
        BOOL fMoreMods = Module32First(hthSnapshot, &me);

        for(; fMoreMods; fMoreMods = Module32Next(hthSnapshot, &me))
        {
            fFound = (_tcscmp(me.szModule,  lpszDllName) == 0) ||
                     (_tcscmp(me.szExePath, lpszDllName) == 0);

            if(fFound) break;
        }

        if(!fFound)
        {
            return FALSE;
        }

        // Get the real address of LoadLibraryW in Kernel32.dll
        PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")), "FreeLibrary");

        if(pfnThreadRtn == NULL)
        {
            return FALSE;
        }

        // Create a remote thread that calls LoadLibraryW(DLLPathname)
        hThread = ::CreateRemoteThread(hPro, NULL, 0, pfnThreadRtn, me.modBaseAddr, 0, NULL);

        if(hThread == NULL)
        {
            return FALSE;
        }

        // Wait for the remote thread to terminate
        WaitForSingleObject(hThread, INFINITE);
        return TRUE;
    }
    __finally   // Now we can clean everything up
    {
        if(hThread != NULL)
        {
            CloseHandle(hThread);
            hThread = NULL;
        }

        if(hthSnapshot != NULL)
        {
            CloseHandle(hthSnapshot);
            hthSnapshot = NULL;
        }

        if(NULL != hPro)
        {
            ::CloseHandle(hPro);
            hPro = NULL;
        }
    }

    return FALSE;
}

BOOL CInjectDll::ApcInjectDll(DWORD dwPID, LPCTSTR lpszDllName)
{
    if(!EnablePrivilege(SE_DEBUG_NAME))
    {
        return FALSE;
    }

    HANDLE hPro =::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

    if(NULL == hPro)
    {
        return FALSE;
    }

    HANDLE hthSnapshot = NULL;

    __try
    {
        int cb  = (_tcslen(lpszDllName) + 1) * sizeof(TCHAR);
        LPVOID pszLibFileRemote = VirtualAllocEx(hPro, NULL, cb, MEM_COMMIT, PAGE_READWRITE);

        if(pszLibFileRemote == NULL)
        {
            return FALSE;
        }

        if(!WriteProcessMemory(hPro, pszLibFileRemote, (PVOID) lpszDllName, cb, NULL))
        {
            return FALSE;
        }

#ifdef _UNICODE
#define Libaray ("LoadLibraryW")
#else
#define Libaray ("LoadLibraryA")
#endif
        PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")), Libaray);

        if(pfnThreadRtn == NULL)
        {
            return FALSE;
        }

        // 创建线程快照
        THREADENTRY32 te32 = { sizeof(THREADENTRY32) } ;
        hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwPID) ;

        if(hthSnapshot == INVALID_HANDLE_VALUE)
            return FALSE ;

        // 枚举所有线程
        if(Thread32First(hthSnapshot, &te32))
        {
            do
            {
                // 判断是否目标进程中的线程
                if(te32.th32OwnerProcessID == dwPID)
                {
                    // 打开线程
                    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID) ;

                    if(hThread)
                    {
                        // 向指定线程添加APC
                        DWORD dwRet = QueueUserAPC((PAPCFUNC)pfnThreadRtn, hThread, (ULONG_PTR)pszLibFileRemote) ;
                        CloseHandle(hThread) ;
                    }
                }
            }
            while(Thread32Next(hthSnapshot, &te32)) ;
        }

        return TRUE;
    }
    __finally   // Now we can clean everything up
    {
        if(hthSnapshot != NULL)
        {
            CloseHandle(hthSnapshot);
            hthSnapshot = NULL;
        }


        if(NULL != hPro)
        {
            ::CloseHandle(hPro);
            hPro = NULL;
        }
    }

    return FALSE;
}

BOOL CInjectDll::ApcUnInjectDll(DWORD dwPID, LPCTSTR lpszDllName)
{
    if(!EnablePrivilege(SE_DEBUG_NAME))
    {
        return FALSE;
    }

    HANDLE hPro =::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

    if(NULL == hPro)
    {
        return FALSE;
    }

    HANDLE hthSnapshot = NULL;

    __try
    {
        PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")), "FreeLibrary");

        if(pfnThreadRtn == NULL)
        {
            return FALSE;
        }

        // 创建线程快照
        THREADENTRY32 te32 = { sizeof(THREADENTRY32) } ;
        hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwPID) ;

        if(hthSnapshot == INVALID_HANDLE_VALUE)
            return FALSE ;

        BOOL bRet = FALSE;

        // 枚举所有线程
        if(Thread32First(hthSnapshot, &te32))
        {
            do
            {
                // 判断是否目标进程中的线程
                if(te32.th32OwnerProcessID == dwPID)
                {
                    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);

                    if(hThread != NULL)
                    {
                        // 枚举线程中模块
                        HANDLE hthSnapshot1 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

                        if(hthSnapshot1 == INVALID_HANDLE_VALUE)
                        {
                            return FALSE;
                        }

                        // Get the HMODULE of the desired library
                        MODULEENTRY32 me = { sizeof(me) };
                        BOOL fFound = FALSE;
                        BOOL fMoreMods = Module32First(hthSnapshot1, &me);

                        for(; fMoreMods; fMoreMods = Module32Next(hthSnapshot1, &me))
                        {
                            fFound = (_tcscmp(me.szModule,  lpszDllName) == 0) ||
                                     (_tcscmp(me.szExePath, lpszDllName) == 0);

                            if(fFound)
                            {
                                bRet = FALSE;
                                DWORD dwRet = QueueUserAPC((PAPCFUNC)pfnThreadRtn, hThread, (ULONG_PTR)me.modBaseAddr);

                                if(dwRet)
                                {
                                    bRet = TRUE;
                                }
                            }
                        }

                        CloseHandle(hthSnapshot1);
                        CloseHandle(hThread) ;
                    }
                }
            }
            while(Thread32Next(hthSnapshot, &te32)) ;
        }

        return bRet;
    }
    __finally   // Now we can clean everything up
    {
        if(hthSnapshot != NULL)
        {
            CloseHandle(hthSnapshot);
            hthSnapshot = NULL;
        }


        if(NULL != hPro)
        {
            ::CloseHandle(hPro);
            hPro = NULL;
        }
    }

    return FALSE;
}
