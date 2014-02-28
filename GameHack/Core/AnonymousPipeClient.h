#pragma once
#include "IAnonymousPipe.h"

class CAnonymousPipeClient : public IAnonymousPipe
{
public:
    CAnonymousPipeClient(void);
    virtual ~CAnonymousPipeClient(void);

    BOOL Create();
    BOOL Read(LPVOID lpBuf, DWORD dwBufSize, LPDWORD dwReaded);
    BOOL Write(LPCVOID lpBuf, DWORD dwBufSize, LPDWORD dwWrited);
    void Close();
private:
    HANDLE m_hRead;
    HANDLE m_hWrite;
};

