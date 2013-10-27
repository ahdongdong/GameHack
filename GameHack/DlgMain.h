#pragma once
class CDlgMain
{
public:
    CDlgMain( void );
    virtual ~CDlgMain( void );
public:
    BOOL Create( HMODULE hDll , HWND hParent );
    void Destroy();
    BOOL Show( BOOL bShow = TRUE );
    BOOL IsShow()const;
    static HWND m_hDlg;
protected:
    static VOID CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
    static INT_PTR CALLBACK DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
    static void SetItemText( HWND hDlg, UINT itemID, LPCTSTR lpszText );
};

