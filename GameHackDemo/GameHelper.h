#pragma once
#include <atlstr.h>

class CGameHelper
{
    typedef BOOL ( *InstallWindowHook )( DWORD dwPID );
    typedef void ( *UnInstallWindowHook )();
	typedef void (*SetConfigPath)(LPCTSTR lpszPath);
public:
    CGameHelper( void );
    virtual ~CGameHelper( void );
    
    BOOL Init();
    void UnInit();
    
    BOOL Start( CString sProName );
    BOOL Stop();
	void SetCfgPath(LPCTSTR lpszPath);
protected:
    BOOL InstallHook( DWORD dwPID );
    BOOL UnInstallHook();
    CString GetGameHackDllPath();
    BOOL IsInited();
    DWORD GetPidFromName( CString sProName );
	DWORD GetThreadIDByProcssID( DWORD dwProcessID );
private:
    BOOL m_bInited;
    HMODULE m_hGameHackDll;
    InstallWindowHook m_InstallWH;
    UnInstallWindowHook m_UnInstallWH;
	SetConfigPath m_SetConfigPath;
};

