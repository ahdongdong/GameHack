#include "stdafx.h"
#include "NamedPipe.h"
#include <assert.h>

CNamedPipe::CNamedPipe()
{
    m_hPipe = INVALID_HANDLE_VALUE;
}

CNamedPipe::~CNamedPipe()
{
    Close();
}

BOOL CNamedPipe::Create(LPCTSTR lpszName, DWORD dwOpenMode, DWORD dwPipeMode,
                        DWORD dwMaxInstances, DWORD dwOutBufferSize,
                        DWORD dwInBufferSize, DWORD dwDefaultTimeOut,
                        LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
    assert(!IsOpen());
    assert(_tcslen(lpszName));
    //the class encapsulates creating the pipe name, all that is required is
    //a simple name for the mailslot e.g. lpName = PJPIPE will create the pipe
    //name \\.\pipe\PJPIPE
    TCHAR pszPipeName[_MAX_PATH];
    _tcscpy_s(pszPipeName, _T("\\\\.\\PIPE\\"));
    _tcscat_s(pszPipeName, lpszName);
    m_hPipe = ::CreateNamedPipe(pszPipeName, dwOpenMode, dwPipeMode, dwMaxInstances,
                                dwOutBufferSize, dwInBufferSize, dwDefaultTimeOut, lpSecurityAttributes);

    if(m_hPipe == INVALID_HANDLE_VALUE)
        ATLTRACE("CNamedPipe::Create() failed, GetLastError returned %d\n", ::GetLastError());

    return (m_hPipe != INVALID_HANDLE_VALUE);
}

BOOL CNamedPipe::Open(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwDesiredAccess,
                      DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                      DWORD dwFlagsAndAttributes)
{
    assert(!IsOpen());
    assert(_tcslen(lpszServerName));
    assert(_tcslen(lpszPipeName));
    //Construct the canonical pipe name
    CString sPipeName;
    sPipeName.Format(_T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);
    m_hPipe = CreateFile(sPipeName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
                         OPEN_EXISTING, dwFlagsAndAttributes, NULL);

    if(m_hPipe == INVALID_HANDLE_VALUE)
        ATLTRACE("CNamedPipe::Open() failed, GetLastError returned %d\n", ::GetLastError());

    return (m_hPipe != INVALID_HANDLE_VALUE);
}

BOOL CNamedPipe::Attach(HANDLE hPipe)
{
    if(m_hPipe != hPipe)
        Close();

    m_hPipe = hPipe;
    return TRUE;
}

HANDLE CNamedPipe::Detach()
{
    HANDLE hReturn = m_hPipe;
    m_hPipe = INVALID_HANDLE_VALUE;
    return hReturn;
}

BOOL CNamedPipe::Close()
{
    BOOL bSuccess = TRUE;

    if(IsOpen())
    {
        bSuccess = ::CloseHandle(m_hPipe);

        if(!bSuccess)
            ATLTRACE("CNamedPipe::Close() failed, GetLastError returned %d\n", ::GetLastError());
    }

    return bSuccess;
}

BOOL CNamedPipe::ConnectClient(LPOVERLAPPED lpOverlapped)
{
    assert(m_hPipe != NULL); //Pipe must be open
#ifdef _DEBUG
    BOOL bServerPipe;
#endif
    assert(IsServerPipe(bServerPipe));
    assert(bServerPipe);     //Must be called from the server side
    BOOL bSuccess = ::ConnectNamedPipe(m_hPipe, lpOverlapped);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::ConnectClient() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::DisconnectClient()
{
    assert(m_hPipe != NULL); //Pipe must be open
#ifdef _DEBUG
    BOOL bServerPipe;
#endif
    assert(IsServerPipe(bServerPipe));
    assert(bServerPipe);     //Must be called from the server side
    BOOL bSuccess = ::DisconnectNamedPipe(m_hPipe);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::DisconnectClient() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::Flush()
{
    assert(m_hPipe != NULL); //Pipe must be open
    BOOL bSuccess = ::FlushFileBuffers(m_hPipe);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::Flush() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,
                       DWORD& dwNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    assert(m_hPipe != NULL); //Pipe must be open
    BOOL bSuccess = ::WriteFile(m_hPipe, lpBuffer, dwNumberOfBytesToWrite,
                                &dwNumberOfBytesWritten, lpOverlapped);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::Write() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,
                       LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    assert(m_hPipe != NULL); //Pipe must be open
    BOOL bSuccess = ::WriteFileEx(m_hPipe, lpBuffer, dwNumberOfBytesToWrite,
                                  lpOverlapped, lpCompletionRoutine);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::Write() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,
                      DWORD& dwNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    assert(m_hPipe != NULL); //Pipe must be open
    BOOL bSuccess = ::ReadFile(m_hPipe, lpBuffer, dwNumberOfBytesToRead,
                               &dwNumberOfBytesRead, lpOverlapped);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::Read() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,
                      LPOVERLAPPED lpOverlapped,  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    assert(m_hPipe != NULL); //Pipe must be open
    BOOL bSuccess = ::ReadFileEx(m_hPipe, lpBuffer, dwNumberOfBytesToRead,
                                 lpOverlapped, lpCompletionRoutine);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::Read() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD& dwBytesRead,
                      DWORD& dwTotalBytesAvail, DWORD& dwBytesLeftThisMessage)
{
    assert(m_hPipe != NULL); //Pipe must be open
    BOOL bSuccess = ::PeekNamedPipe(m_hPipe, lpBuffer, dwBufferSize, &dwBytesRead,
                                    &dwTotalBytesAvail, &dwBytesLeftThisMessage);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::Peek() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::Transact(LPVOID lpInBuffer, DWORD dwInBufferSize, LPVOID lpOutBuffer,
                          DWORD dwOutBufferSize, DWORD& dwBytesRead, LPOVERLAPPED lpOverlapped)
{
    assert(m_hPipe != NULL); //Pipe must be open
    BOOL bSuccess = ::TransactNamedPipe(m_hPipe, lpInBuffer, dwInBufferSize, lpOutBuffer,
                                        dwOutBufferSize, &dwBytesRead, lpOverlapped);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::Transact() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::IsBlockingPipe(BOOL& bIsBlocking) const
{
    assert(m_hPipe != NULL); //Pipe must be open
    DWORD dwState;
    BOOL bSuccess = ::GetNamedPipeHandleState(m_hPipe, &dwState, NULL, NULL, NULL, NULL, 0);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::IsBlockingPipe() failed, GetLastError returned %d\n", ::GetLastError());
    else
        bIsBlocking = ((dwState & PIPE_NOWAIT) == 0);

    return bSuccess;
}

BOOL CNamedPipe::IsClientPipe(BOOL& bClientPipe) const
{
    assert(m_hPipe != NULL); //Pipe must be open
    DWORD dwFlags;
    BOOL bSuccess = ::GetNamedPipeInfo(m_hPipe, &dwFlags, NULL, NULL, NULL);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::IsClientPipe() failed, GetLastError returned %d\n", ::GetLastError());
    else
        bClientPipe = ((dwFlags & PIPE_CLIENT_END) != 0);

    return bSuccess;
}

BOOL CNamedPipe::IsServerPipe(BOOL& bServerPipe) const
{
    assert(m_hPipe != NULL); //Pipe must be open
    DWORD dwFlags;
    BOOL bSuccess = ::GetNamedPipeInfo(m_hPipe, &dwFlags, NULL, NULL, NULL);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::IsServerPipe() failed, GetLastError returned %d\n", ::GetLastError());
    else
        bServerPipe = ((dwFlags & PIPE_SERVER_END) != 0);

    return bSuccess;
}

BOOL CNamedPipe::IsMessagePipe(BOOL& bMessagePipe) const
{
    assert(m_hPipe != NULL); //Pipe must be open
    DWORD dwState;
    BOOL bSuccess = ::GetNamedPipeHandleState(m_hPipe, &dwState, NULL, NULL, NULL, NULL, 0);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::IsMessagePipe() failed, GetLastError returned %d\n", ::GetLastError());
    else
        bMessagePipe = ((dwState & PIPE_READMODE_MESSAGE) != 0);

    return bSuccess;
}

DWORD CNamedPipe::GetCurrentInstances() const
{
    assert(m_hPipe != NULL); //Pipe must be open
    DWORD dwCurInstances = 0;
    BOOL bSuccess = ::GetNamedPipeHandleState(m_hPipe, NULL, &dwCurInstances, NULL, NULL, NULL, 0);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::GetCurrentInstances() failed, GetLastError returned %d\n", ::GetLastError());

    return dwCurInstances;
}

DWORD CNamedPipe::GetMaxCollectionCount() const
{
    assert(m_hPipe != NULL); //Pipe must be open
#ifdef _DEBUG
    BOOL bClientPipe;
#endif
    assert(IsClientPipe(bClientPipe));
    assert(bClientPipe);     //Must be called from the client side
    DWORD dwMaxCollectionCount = 0;
    BOOL bSuccess = ::GetNamedPipeHandleState(m_hPipe, NULL, NULL, &dwMaxCollectionCount, NULL, NULL, 0);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::GetMaxCollectionCount() failed, GetLastError returned %d\n", ::GetLastError());

    return dwMaxCollectionCount;
}

DWORD CNamedPipe::GetCollectionTimeout() const
{
    assert(m_hPipe != NULL); //Pipe must be open
#ifdef _DEBUG
    BOOL bClientPipe;
#endif
    assert(IsClientPipe(bClientPipe));
    assert(bClientPipe);     //Must be called from the client side
    DWORD dwCollectDataTimeout = 0;
    BOOL bSuccess = ::GetNamedPipeHandleState(m_hPipe, NULL, NULL, NULL, &dwCollectDataTimeout, NULL, 0);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::GetCollectionTimeout() failed, GetLastError returned %d\n", ::GetLastError());

    return dwCollectDataTimeout;
}

DWORD CNamedPipe::GetOutboundBufferSize() const
{
    assert(m_hPipe != NULL); //Pipe must be open
    DWORD dwOutBufferSize = 0;
    BOOL bSuccess = ::GetNamedPipeInfo(m_hPipe, NULL, &dwOutBufferSize, NULL, NULL);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::GetOutboundBufferSize() failed, GetLastError returned %d\n", ::GetLastError());

    return dwOutBufferSize;
}

DWORD CNamedPipe::GetInboundBufferSize() const
{
    assert(m_hPipe != NULL); //Pipe must be open
    DWORD dwInBufferSize = 0;
    BOOL bSuccess = ::GetNamedPipeInfo(m_hPipe, NULL, NULL, &dwInBufferSize, NULL);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::GetInboundBufferSize() failed, GetLastError returned %d\n", ::GetLastError());

    return dwInBufferSize;
}

CString CNamedPipe::GetClientUserName() const
{
    assert(m_hPipe != NULL); //Pipe must be open
#ifdef _DEBUG
    BOOL bServerPipe;
#endif
    assert(IsServerPipe(bServerPipe));
    assert(bServerPipe);     //Must be called from the server side
    TCHAR pszUserName[_MAX_PATH];
    BOOL bSuccess = ::GetNamedPipeHandleState(m_hPipe, NULL, NULL, NULL, NULL, pszUserName, _MAX_PATH);
    CString sName;

    if(!bSuccess)
        ATLTRACE("CNamedPipe::GetClientUserName() failed, GetLastError returned %d\n", ::GetLastError());
    else
        sName = pszUserName;

    return sName;
}

DWORD CNamedPipe::GetMaxInstances() const
{
    assert(m_hPipe != NULL); //Pipe must be open
    DWORD dwMaxInstances = 0;
    BOOL bSuccess = ::GetNamedPipeInfo(m_hPipe, NULL, NULL, NULL, &dwMaxInstances);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::GetMaxInstances() failed, GetLastError returned %d\n", ::GetLastError());

    return dwMaxInstances;
}

BOOL CNamedPipe::SetMode(BOOL bByteMode, BOOL bBlockingMode)
{
    assert(m_hPipe != NULL); //Pipe must be open
    DWORD dwMode;

    if(bByteMode)
    {
        if(bBlockingMode)
            dwMode = PIPE_READMODE_BYTE | PIPE_WAIT;
        else
            dwMode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
    }
    else
    {
        if(bBlockingMode)
            dwMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
        else
            dwMode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
    }

    BOOL bSuccess = ::SetNamedPipeHandleState(m_hPipe, &dwMode, NULL, NULL);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::SetMode() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::SetMaxCollectionCount(DWORD dwCollectionCount)
{
    assert(m_hPipe != NULL); //Pipe must be open
#ifdef _DEBUG
    BOOL bClientPipe;
#endif
    assert(IsClientPipe(bClientPipe));
    assert(bClientPipe);     //Must be called from the client side
    BOOL bSuccess = ::SetNamedPipeHandleState(m_hPipe, NULL, &dwCollectionCount, NULL);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::SetMaxCollectionCount() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::SetCollectionTimeout(DWORD dwDataTimeout)
{
    assert(m_hPipe != NULL); //Pipe must be open
#ifdef _DEBUG
    BOOL bClientPipe;
#endif
    assert(IsClientPipe(bClientPipe));
    assert(bClientPipe);     //Must be called from the client side
    BOOL bSuccess = ::SetNamedPipeHandleState(m_hPipe, NULL, NULL, &dwDataTimeout);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::SetCollectionTimeout() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::Call(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, LPVOID lpInBuffer,
                      DWORD dwInBufferSize, LPVOID lpOutBuffer, DWORD dwOutBufferSize,
                      DWORD& dwBytesRead, DWORD dwTimeOut)
{
    //Construct the canonical pipe name
    CString sPipeName;
    sPipeName.Format(_T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);
    BOOL bSuccess = ::CallNamedPipe(sPipeName, lpInBuffer, dwInBufferSize, lpOutBuffer,
                                    dwOutBufferSize, &dwBytesRead, dwTimeOut);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::Call() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}

BOOL CNamedPipe::ServerAvailable(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwTimeOut)
{
    //Construct the canonical pipe name
    CString sPipeName;
    sPipeName.Format(_T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);
    BOOL bSuccess = ::WaitNamedPipe(sPipeName, dwTimeOut);

    if(!bSuccess)
        ATLTRACE("CNamedPipe::ServerAvailable() failed, GetLastError returned %d\n", ::GetLastError());

    return bSuccess;
}
