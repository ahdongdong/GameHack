#include "stdafx.h"
#include "AnonymousPipeClient.h"


CAnonymousPipeClient::CAnonymousPipeClient(void): m_hRead(NULL), m_hWrite(NULL)
{
}


CAnonymousPipeClient::~CAnonymousPipeClient(void)
{
}

BOOL CAnonymousPipeClient::Create()
{
    Close();
    m_hRead = GetStdHandle(STD_INPUT_HANDLE);
    m_hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
    return ((NULL != m_hWrite) && (NULL != m_hRead));
}

BOOL CAnonymousPipeClient::Read(LPVOID lpBuf, DWORD dwBufSize, LPDWORD dwReaded)
{
    if(NULL == m_hRead)
        return FALSE;

    return ::ReadFile(m_hRead, lpBuf, dwBufSize, dwReaded, NULL);
}

BOOL CAnonymousPipeClient::Write(LPCVOID lpBuf, DWORD dwBufSize, LPDWORD dwWrited)
{
    if(NULL == m_hWrite)
        return FALSE;

    return ::WriteFile(m_hWrite, lpBuf, dwBufSize, dwWrited, NULL);
}

void CAnonymousPipeClient::Close()
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
