#pragma once
#include "GamePlayer.h"
#include "ConfigFile.h"
#include "ProcessHelper.h"

class CDlgMain
{
public:
    CDlgMain(void);
    virtual ~CDlgMain(void);
public:
    BOOL Create(HMODULE hDll , HWND hParent, DWORD dwPID);
    void Close();
    BOOL Show(BOOL bShow = TRUE);
    BOOL IsShow()const;
protected:
    CString GetConfigPath() const;
    void Destroy();
    static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static void SetItemText(HWND hDlg, UINT itemID, LPCTSTR lpszText);
private:
    static CDlgMain* m_pThis;
    HWND m_hDlg;

    CConfigFile m_configFile;
    CProcessHelper m_proHlp;
    CGamePlayer* m_pPlayer;
};

