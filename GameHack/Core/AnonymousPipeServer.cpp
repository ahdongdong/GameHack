#include "stdafx.h"
#include "AnonymousPipeServer.h"


CAnonymousPipeServer::CAnonymousPipeServer(const PIPECLIENTINFO* pClientInfo = NULL)
    : m_hWrite(NULL), m_hRead(NULL), m_pClientInfo(pClientInfo)
{
}


CAnonymousPipeServer::~CAnonymousPipeServer(void)
{
    Close();
}

BOOL CAnonymousPipeServer::Create()
{
    // 服务端必须指定子进程路径，以便构成父子关系
    if(NULL == m_pClientInfo)
    {
        return FALSE;
    }

    Close();
    SECURITY_ATTRIBUTES se = {0};
    se.nLength = sizeof(se);
    se.bInheritHandle = TRUE;
    se.lpSecurityDescriptor = m_pClientInfo->m_lpSecurityDescriptor;

    if(!CreatePipe(&m_hRead, &m_hWrite, &se, 0))
    {
        return FALSE;
    }

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = m_hRead;
    si.hStdOutput = m_hWrite;
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
//    USES_CONVERSION;
    BOOL bRet = CreateProcess(m_pClientInfo->m_sAppPath, const_cast<TCHAR*>(m_pClientInfo->m_sCmdLine),
                              NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

    if(bRet)
    {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return TRUE;
    }

    Close();
    return FALSE;
}

BOOL CAnonymousPipeServer::Read(LPVOID lpBuf, DWORD dwBufSize, LPDWORD dwReaded)
{
    if(NULL == m_hRead)
        return FALSE;

    return ::ReadFile(m_hRead, lpBuf, dwBufSize, dwReaded, NULL);
}

BOOL CAnonymousPipeServer::Write(LPCVOID lpBuf, DWORD dwBufSize, LPDWORD dwWrited)
{
    if(NULL == m_hWrite)
        return FALSE;

    return ::WriteFile(m_hWrite, lpBuf, dwBufSize, dwWrited, NULL);
}

void CAnonymousPipeServer::Close()
{
    if(NULL != m_hRead)
    {
        CloseHandle(m_hRead);
        m_hRead = NULL;
    }

    if(NULL != m_hWrite)
    {
        CloseHandle(m_hWrite);
        m_hWrite = NULL;
    }
}