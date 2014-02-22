#include "stdafx.h"
#include "WinHook.h"
#include "GameUtility.h"

CWinHook::CWinHook(HMODULE hModule): CWindowsHook(hModule)
{
    m_pDlgMain = NULL;
}


CWinHook::~CWinHook(void)
{
    if(NULL != m_pDlgMain)
    {
        delete m_pDlgMain;
        m_pDlgMain = NULL;
    }
}

BOOL CWinHook::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    BOOL bKeyUp = lParam & (1 << 31);

    if((bKeyUp) && (wParam == VK_F8))
    {
        if(NULL == m_pDlgMain)
        {
            m_pDlgMain = new CDlgMain();
        }

        HWND hParent = GetHwndFromPID(GetCurrentProcessId());
        m_pDlgMain->Create(m_hModule, hParent, GetCurrentProcessId());
        m_pDlgMain->Show(m_pDlgMain->IsShow() ? FALSE : TRUE);
        return TRUE;
    }

    return FALSE;
}
