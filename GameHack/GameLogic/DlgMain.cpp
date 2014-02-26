#include "StdAfx.h"
#include <Windows.h>
#include "DlgMain.h"
#include "..\resource.h"
#include "GamePlayer.h"

CDlgMain* CDlgMain::m_pThis = NULL;
CDlgMain::CDlgMain(void): m_hDlg(NULL), m_pPlayer(NULL)
{
    m_pThis = this;
}

CDlgMain::~CDlgMain(void)
{
    Close();
}

CString CDlgMain::GetConfigPath() const
{
	const CString sCfgPath = _T("H:\\Project\\GameHack\\bin\\Debug\\GameHack.ini");
	return sCfgPath;

    TCHAR sModulePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, sModulePath, MAX_PATH);
    CString sModule = sModulePath;
    sModule.Truncate(sModule.ReverseFind(_T('\\')) + 1);
    CString sPath = sModule + _T("GameHack.ini");
	return sPath;
}

BOOL CDlgMain::Create(HMODULE hDll, HWND hParent, DWORD dwPID)
{
    if(NULL != m_hDlg)
    {
        return TRUE;
    }

	CString sCfgPath=GetConfigPath();

    if(!m_configFile.Load(sCfgPath))
    {
        return FALSE;
    }

    if(!m_proHlp.Open(dwPID))
    {
        return FALSE;
    }

    m_pPlayer = new CGamePlayer(&m_proHlp, &m_configFile);
    m_hDlg = CreateDialogParam(hDll, MAKEINTRESOURCE(IDD_DLG_MAIN), hParent, (DLGPROC)DlgProc, (LPARAM)this) ;
    return TRUE;
}

BOOL CDlgMain::Show(BOOL bShow/*=TRUE*/)
{
    if(!IsWindow(m_hDlg))
    {
        return FALSE;
    }

    return ::ShowWindow(m_hDlg, bShow ? SW_SHOWNORMAL : SW_HIDE);
}

BOOL CDlgMain::IsShow() const
{
    if(!IsWindow(m_hDlg))
    {
        return FALSE;
    }

    return ::IsWindowVisible(m_hDlg);
}

INT_PTR CALLBACK  CDlgMain::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_CLOSE:
        {
            m_pThis->Show(FALSE);
            m_pThis->Close();
            return (TRUE);
        }

        case WM_TIMER:
        {
            TimerProc(hDlg, WM_TIMER, wParam, 0);
            return TRUE;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_START:
                {
                    ::SetTimer(hDlg, 0, 500, NULL);
                    return TRUE;
                }

                case IDC_STOP:
                {
                    ::KillTimer(hDlg, 0);
                    return TRUE;
                }
            }

            return (FALSE);
    }

    return FALSE;
}

void CDlgMain::Destroy()
{
    if(NULL != m_pPlayer)
    {
        delete m_pPlayer;
        m_pPlayer = NULL;
    }

    m_configFile.UnLoad();
    m_proHlp.Close();

    if(NULL != m_hDlg)
    {
        if(!IsWindow(m_hDlg))
        {
            return ;
        }

        DestroyWindow(m_hDlg);
        m_hDlg = NULL;
    }
}

VOID CALLBACK CDlgMain::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    if(0 == idEvent)
    {
        CString sCurAxis = m_pThis->m_pPlayer->GetCurAxis();
        SetItemText(hwnd, IDC_STATIC_CURAXIS, sCurAxis);

        CString sPlayerName = m_pThis->m_pPlayer->GetPlayerName();
        SetItemText(hwnd, IDC_STATIC_MC, sPlayerName);
    }
}

void CDlgMain::SetItemText(HWND hDlg, UINT itemID, LPCTSTR lpszText)
{
    HWND hCtrl = GetDlgItem(hDlg, itemID);
    SetWindowText(hCtrl, lpszText);
}

void CDlgMain::Close()
{
    Destroy();
}
