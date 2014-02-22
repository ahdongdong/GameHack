#include "StdAfx.h"
#include "GameHelper.h"
#include <TlHelp32.h>


CGameHelper::CGameHelper(void)
{
    m_hGameHackDll = NULL;
    m_InstallWH = NULL;
    m_UnInstallWH = NULL;
}


CGameHelper::~CGameHelper(void)
{
    Stop();
}


DWORD CGameHelper::GetThreadIDByProcssID(DWORD dwProcessID)
{
    DWORD dwThreadID = -1;
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

BOOL CGameHelper::Start(CString sProName)
{
    if(!Init()) return FALSE;

    DWORD dwPID = GetPidFromName(sProName);

    if(-1 == dwPID)
    {
        return FALSE;
    }

    return InstallHook(dwPID);
}

BOOL CGameHelper::Stop()
{
    BOOL bRet = UnInstallHook();
    UnInit();
    return bRet;
}

DWORD CGameHelper::GetPidFromName(CString sProName)
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

BOOL CGameHelper::Init()
{
    CString sDllPath = GetGameHackDllPath();

    if(!PathFileExists(sDllPath))
    {
        return FALSE;
    }

    if(NULL != m_hGameHackDll && NULL != m_InstallWH && NULL != m_UnInstallWH)
    {
        return TRUE;
    }

    if(NULL == m_hGameHackDll)
    {
        m_hGameHackDll =::LoadLibrary(sDllPath);
    }

    if(NULL != m_hGameHackDll)
    {
        m_InstallWH = (InstallWindowHook)GetProcAddress(m_hGameHackDll, "InstallWindowHook");
        m_UnInstallWH = (UnInstallWindowHook)GetProcAddress(m_hGameHackDll, "UnInstallWindowHook");

        if((NULL != m_InstallWH) && (NULL != m_UnInstallWH))
        {
            return TRUE;
        }
    }

    return FALSE;
}

void CGameHelper::UnInit()
{
    if(NULL != m_hGameHackDll)
    {
        ::FreeLibrary(m_hGameHackDll);
        m_hGameHackDll = NULL;
        m_InstallWH = NULL;
        m_UnInstallWH = NULL;
    }
}

CString CGameHelper::GetGameHackDllPath()
{
    CString sDllPath;
    TCHAR sModulePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, sModulePath, MAX_PATH);
    CString sModule = sModulePath;
    sModule.Truncate(sModule.ReverseFind(_T('\\')) + 1);
    sDllPath = sModule + _T("GameHack.dll");
    return sDllPath;
}

BOOL CGameHelper::InstallHook(DWORD dwPID)
{
    if(NULL != m_InstallWH)
    {
        return m_InstallWH(dwPID);
    }

    return FALSE;
}

BOOL CGameHelper::UnInstallHook()
{
    if(NULL != m_UnInstallWH)
    {
        m_UnInstallWH();
    }

    return TRUE;
}
