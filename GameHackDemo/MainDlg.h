// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GameHelper.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
    public CMessageFilter, public CIdleHandler
{
private:
    CGameHelper m_gameHelper;
public:
    enum { IDD = IDD_MAINDLG };
    
    virtual BOOL PreTranslateMessage( MSG* pMsg )
    {
        return CWindow::IsDialogMessage( pMsg );
    }
    
    virtual BOOL OnIdle()
    {
        UIUpdateChildWindows();
        return FALSE;
    }
    
    BEGIN_UPDATE_UI_MAP( CMainDlg )
    END_UPDATE_UI_MAP()
    
    BEGIN_MSG_MAP( CMainDlg )
    MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
    MESSAGE_HANDLER( WM_DESTROY, OnDestroy )
    COMMAND_ID_HANDLER( ID_APP_ABOUT, OnAppAbout )
    COMMAND_ID_HANDLER( IDOK, OnOK )
    COMMAND_ID_HANDLER( IDCANCEL, OnCancel )
    COMMAND_HANDLER( IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1 )
    COMMAND_HANDLER( IDC_BUTTON2, BN_CLICKED, OnBnClickedButton2 )
    END_MSG_MAP()
    
    // Handler prototypes (uncomment arguments if needed):
    //	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    //	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    //	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
    
    LRESULT OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
    {
        // center the dialog on the screen
        CenterWindow();
        
        // set icons
        HICON hIcon = AtlLoadIconImage( IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics( SM_CXICON ), ::GetSystemMetrics( SM_CYICON ) );
        SetIcon( hIcon, TRUE );
        HICON hIconSmall = AtlLoadIconImage( IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ) );
        SetIcon( hIconSmall, FALSE );
        
        // register object for message filtering and idle updates
        CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT( pLoop != NULL );
        pLoop->AddMessageFilter( this );
        pLoop->AddIdleHandler( this );
        
        UIAddChildWindowContainer( m_hWnd );
        
        m_gameHelper.Init();
        
        return TRUE;
    }
    
    LRESULT OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
    {
        // unregister message filtering and idle updates
        CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT( pLoop != NULL );
        pLoop->RemoveMessageFilter( this );
        pLoop->RemoveIdleHandler( this );
        
		m_gameHelper.Stop();
        m_gameHelper.UnInit();
        return 0;
    }
    
    LRESULT OnAppAbout( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
    {
        CAboutDlg dlg;
        dlg.DoModal();
        return 0;
    }
    
    LRESULT OnOK( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
    {
        // TODO: Add validation code
        CloseDialog( wID );
        return 0;
    }
    
    LRESULT OnCancel( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
    {
        CloseDialog( wID );
        return 0;
    }
    
    void CloseDialog( int nVal )
    {
        DestroyWindow();
        ::PostQuitMessage( nVal );
    }
    
    void GetConfigPath( CString& sPath ) const
    {
        TCHAR sModulePath[MAX_PATH] = {0};
        GetModuleFileName( NULL, sModulePath, MAX_PATH );
        CString sModule = sModulePath;
        sModule.Truncate( sModule.ReverseFind( _T( '\\' ) ) + 1 );
        sPath = sModule + _T( "GameHack.ini" );
    }
    
    LRESULT OnBnClickedButton1( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
    {
        CString sConfig;
        GetConfigPath( sConfig );
        m_gameHelper.SetCfgPath( sConfig );
        const CString sGameName = _T( "asktao.mod" );
//		const CString sGameName = _T( "notepad++.exe" );
        CString sInfo;
        if ( m_gameHelper.Start( sGameName ) )
        {
            sInfo = _T( "按F8呼出外挂窗口" );
        }
        else
        {
            sInfo = _T( "外挂启动失败" );
        }
        CStatic ctrlInfo = GetDlgItem( IDC_STATIC_INFO );
        ctrlInfo.SetWindowText( sInfo );
        return 0;
    }
    
    LRESULT OnBnClickedButton2( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
    {
        m_gameHelper.Stop();
        
        CStatic ctrlInfo = GetDlgItem( IDC_STATIC_INFO );
        ctrlInfo.SetWindowText( _T( "" ) );
        return 0;
    }
};
