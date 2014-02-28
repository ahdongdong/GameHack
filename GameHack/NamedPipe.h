#pragma once

class CNamedPipe
{
public:
    //Constructors / Destructors
    CNamedPipe();
    ~CNamedPipe();

    //Creation & Opening
    BOOL Create(LPCTSTR lpszName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD dwMaxInstances, DWORD dwOutBufferSize,
                DWORD dwInBufferSize, DWORD dwDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
    BOOL Open(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwDesiredAccess,
              DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
              DWORD dwFlagsAndAttributes = 0);
    operator HANDLE() const
    {
        return m_hPipe;
    };
    BOOL Close();
    BOOL Attach(HANDLE hPipe);
    HANDLE Detach();

    //General functions
    BOOL ConnectClient(LPOVERLAPPED lpOverlapped = NULL);
    BOOL DisconnectClient();
    BOOL Flush();
    BOOL Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,
               DWORD& dwNumberOfBytesWritten, LPOVERLAPPED lpOverlapped = NULL);
    BOOL Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,
               LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
    BOOL Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,
              DWORD& dwNumberOfBytesRead, LPOVERLAPPED lpOverlapped = NULL);
    BOOL Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,
              LPOVERLAPPED lpOverlapped,  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
    BOOL Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD& dwBytesRead,
              DWORD& dwTotalBytesAvail, DWORD& dwBytesLeftThisMessage);
    BOOL Transact(LPVOID lpInBuffer, DWORD dwInBufferSize, LPVOID lpOutBuffer,
                  DWORD dwOutBufferSize, DWORD& dwBytesRead, LPOVERLAPPED lpOverlapped = NULL);

    //State accessors
    BOOL    IsOpen() const
    {
        return (m_hPipe != INVALID_HANDLE_VALUE);
    };
    BOOL    IsBlockingPipe(BOOL& bIsBlocking) const;
    BOOL    IsClientPipe(BOOL& bClientPipe) const;
    BOOL    IsServerPipe(BOOL& bServerPipe) const;
    BOOL    IsMessagePipe(BOOL& bMessagePipe) const;
    DWORD   GetCurrentInstances() const;
    DWORD   GetMaxCollectionCount() const;
    DWORD   GetCollectionTimeout() const;
    DWORD   GetOutboundBufferSize() const;
    DWORD   GetInboundBufferSize() const;
    CString GetClientUserName() const;
    DWORD   GetMaxInstances() const;

    //State accessors
    BOOL SetMode(BOOL bByteMode, BOOL bBlockingMode);
    BOOL SetMaxCollectionCount(DWORD dwCollectionCount);
    BOOL SetCollectionTimeout(DWORD dwDataTimeout);

    //Static functions i.e. named pipe functions which do not take a pipe handle
    static BOOL Call(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, LPVOID lpInBuffer, DWORD dwInBufferSize, LPVOID lpOutBuffer,
                     DWORD dwOutBufferSize, DWORD& dwBytesRead, DWORD dwTimeOut);
    static BOOL ServerAvailable(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwTimeOut);
    HANDLE m_hPipe;
};