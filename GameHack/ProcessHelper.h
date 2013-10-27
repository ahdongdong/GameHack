#pragma once
#include <Windows.h>

class CProcessHelper
{
public:
    CProcessHelper( DWORD dwID );
    virtual ~CProcessHelper( void );
    
public:
    BOOL Open();
    BOOL Close();
    BOOL Terminal();
    BOOL ReadMemory( LPCVOID lpAddr, LPVOID lpBuf, DWORD dwReadSize );
    BOOL WriteMemory( LPVOID lpAddr, LPCVOID lpBuf, DWORD dwWriteSize );
    
    DWORD GetPID()const;
    HANDLE GetHandle();
protected:
    BOOL EnablePrivilege( LPCTSTR PrivilegeName );
private:
    DWORD m_dwID;
    HANDLE m_hProcess;
};

