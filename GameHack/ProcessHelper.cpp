#include "stdafx.h"
#include "ProcessHelper.h"
#include "GameUtility.h"


CProcessHelper::CProcessHelper()
{
    m_hProcess = NULL;
}


CProcessHelper::~CProcessHelper(void)
{
    Close();
}

BOOL CProcessHelper::Open(DWORD dwPID ,DWORD dwDesiredAccess)
{
    EnablePrivilege(SE_DEBUG_NAME);

    if(NULL == m_hProcess)
    {
        m_hProcess =::OpenProcess(dwDesiredAccess, FALSE, dwPID);

        if(NULL != m_hProcess)
        {
            m_dwID = dwPID;
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CProcessHelper::Close()
{
    if(NULL != m_hProcess)
    {
        ::CloseHandle(m_hProcess);
        m_hProcess = NULL;
    }

    return TRUE;
}

BOOL CProcessHelper::Terminal()
{
    BOOL bRet = FALSE;

    if(NULL != m_hProcess)
    {
        bRet =::TerminateProcess(m_hProcess, 0);
    }

    return bRet;
}

BOOL CProcessHelper::ReadMemory(LPCVOID lpAddr, LPVOID lpBuf, DWORD dwReadSize)
{
    BOOL bRet = FALSE;
    DWORD dwReaded = 0;

    if(NULL != m_hProcess)
    {
        bRet =::ReadProcessMemory(m_hProcess, lpAddr, lpBuf, dwReadSize, &dwReaded);
    }

    return ((TRUE == bRet) && (dwReadSize == dwReaded));
}

BOOL CProcessHelper::WriteMemory(LPVOID lpAddr, LPCVOID lpBuf, DWORD dwWriteSize)
{
    BOOL bRet = FALSE;
    DWORD dwWrited = 0;

    if(NULL != m_hProcess)
    {
        bRet =::WriteProcessMemory(m_hProcess, lpAddr, lpBuf, dwWriteSize, &dwWrited);
    }

    return ((TRUE == bRet) && (dwWrited == dwWriteSize));
}

DWORD CProcessHelper::GetPID()const
{
    return m_dwID;
}

HANDLE CProcessHelper::GetModuleHandle()
{
    return m_hProcess;
}

BOOL CProcessHelper::Suspend()
{
    return ::SuspendThread(m_hProcess);
}

BOOL CProcessHelper::Resume()
{
    return ::ResumeThread(m_hProcess);
}

DWORD CProcessHelper::GetMainThreadID()
{
    return GetMTIDFromPID(m_dwID);
}

LPVOID CProcessHelper::VirtualAllocEx(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
{
    return ::VirtualAllocEx(m_hProcess, lpAddress, dwSize, flAllocationType, flProtect);
}

BOOL CProcessHelper::VirtualFreeEx(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)
{
    return ::VirtualFreeEx(m_hProcess, lpAddress, dwSize, dwFreeType);
}

HANDLE CProcessHelper::CreateThread( LPTHREAD_START_ROUTINE lpAddr,LPVOID dwThreadParam )
{
	return ::CreateRemoteThread(m_hProcess, NULL, NULL, lpAddr, dwThreadParam, 0, NULL);
}
