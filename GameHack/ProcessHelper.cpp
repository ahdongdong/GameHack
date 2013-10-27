#include "stdafx.h"
#include "ProcessHelper.h"


CProcessHelper::CProcessHelper( DWORD dwID )
{
    m_dwID = dwID;
    m_hProcess = NULL;
}


CProcessHelper::~CProcessHelper( void )
{
    Close();
}

BOOL CProcessHelper::Open()
{
    EnablePrivilege( SE_DEBUG_NAME );
    if ( NULL == m_hProcess )
    {
        m_hProcess =::OpenProcess( PROCESS_ALL_ACCESS, FALSE, m_dwID );
    }
    
    return ( NULL != m_hProcess );
}

BOOL CProcessHelper::Close()
{
    if ( NULL != m_hProcess )
    {
        ::CloseHandle( m_hProcess );
        m_hProcess = NULL;
    }
    
    return TRUE;
}

BOOL CProcessHelper::Terminal()
{
    BOOL bRet = FALSE;
    
    if ( NULL != m_hProcess )
    {
        bRet =::TerminateProcess( m_hProcess, 0 );
    }
    
    return bRet;
}

BOOL CProcessHelper::ReadMemory( LPCVOID lpAddr, LPVOID lpBuf, DWORD dwReadSize )
{
    BOOL bRet = FALSE;
    DWORD dwReaded = 0;
    
    if ( NULL != m_hProcess )
    {
        bRet =::ReadProcessMemory( m_hProcess, lpAddr, lpBuf, dwReadSize, &dwReaded );
    }
    
    return ( ( TRUE == bRet ) && ( dwReadSize == dwReaded ) );
}

BOOL CProcessHelper::WriteMemory( LPVOID lpAddr, LPCVOID lpBuf, DWORD dwWriteSize )
{
    BOOL bRet = FALSE;
    DWORD dwWrited = 0;
    
    if ( NULL != m_hProcess )
    {
        bRet =::WriteProcessMemory( m_hProcess, lpAddr, lpBuf, dwWriteSize, &dwWrited );
    }
    
    return ( ( TRUE == bRet ) && ( dwWrited == dwWriteSize ) );
}

DWORD CProcessHelper::GetPID()const
{
    return m_dwID;
}

HANDLE CProcessHelper::GetHandle()
{
    return m_hProcess;
}


BOOL CProcessHelper::EnablePrivilege( LPCTSTR PrivilegeName )
{
    HANDLE tokenhandle;
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    
    if ( !OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &tokenhandle ) )
    {
        return FALSE;
    }
    
    if ( !LookupPrivilegeValue( NULL, PrivilegeName, &tp.Privileges[0].Luid ) )
    {
        CloseHandle( tokenhandle );
        return FALSE;
    }
    
    if ( !AdjustTokenPrivileges( tokenhandle, FALSE, &tp, sizeof( tp ), NULL, NULL ) )
    {
        CloseHandle( tokenhandle );
        return FALSE;
    }
    
    CloseHandle( tokenhandle );
    return TRUE;
}

