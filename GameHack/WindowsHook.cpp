#include "stdafx.h"
#include "WindowsHook.h"
#include "GameUtility.h"

#pragma data_seg(".GameHack")
HHOOK CWindowsHook::m_hHook = NULL;
CWindowsHook* g_pHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/Section:.GameHack,RWS")

CWindowsHook* CWindowsHook::m_pThis = NULL;
CWindowsHook::CWindowsHook(HINSTANCE hInst)
{
    m_pThis = this;
    m_hModule = hInst;
}

CWindowsHook::~CWindowsHook(void)
{

}

BOOL CWindowsHook::Install(int iHook, DWORD dwPID)
{
    if(NULL == m_hHook)
    {
        if(-1 != dwPID)
        {
            m_hHook =::SetWindowsHookEx(iHook, (HOOKPROC)_HookProc, m_hModule, GetThreadIDByProcssID(dwPID));
        }
        else
            m_hHook =::SetWindowsHookEx(iHook, (HOOKPROC)_HookProc, m_hModule, 0);
    }

    if(NULL != m_hHook)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CWindowsHook::UnInstall()
{
    BOOL bRet = FALSE;

    if(NULL != m_hHook)
    {
        bRet =::UnhookWindowsHookEx(m_hHook);
        m_hHook = NULL;
    }

    return bRet;
}

LRESULT CALLBACK CWindowsHook::_HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRet =::CallNextHookEx(m_hHook , nCode, wParam, lParam);

    if(nCode == HC_ACTION)
    {
        if(m_pThis->HookProc(nCode, wParam, lParam))
        {
            return TRUE;
        }
    }

    return lRet;
}

BOOL CWindowsHook::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return FALSE;
}