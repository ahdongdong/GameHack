#pragma once

#include "IAnonymousPipe.h"
class CAnonymousPipeServer :public IAnonymousPipe
{
public:
	CAnonymousPipeServer(const PIPECLIENTINFO* clientInfo);
	virtual ~CAnonymousPipeServer(void);

	BOOL Create();
	BOOL Read(LPVOID lpBuf, DWORD dwBufSize, LPDWORD dwReaded) ;
	BOOL Write(LPCVOID lpBuf, DWORD dwBufSize, LPDWORD dwWrited);
	void Close();
private:
	HANDLE m_hRead;
	HANDLE m_hWrite;
	const PIPECLIENTINFO* m_pClientInfo;
};

