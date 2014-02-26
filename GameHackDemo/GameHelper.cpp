#include "StdAfx.h"
#include "GameHelper.h"
#include <TlHelp32.h>
#include "..\GameHack\Core\GameUtility.h"


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
