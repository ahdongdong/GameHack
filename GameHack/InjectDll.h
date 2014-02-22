#pragma once
class CInjectDll
{
public:
    CInjectDll(void);
    virtual ~CInjectDll(void);

    BOOL RemoteInjectDll(DWORD dwPID, LPCTSTR lpszDllName);
    BOOL RemoteUnInjectDll(DWORD dwPID, LPCTSTR lpszDllName);

	BOOL ApcInjectDll(DWORD dwPID, LPCTSTR lpszDllName);
	BOOL ApcUnInjectDll(DWORD dwPID, LPCTSTR lpszDllName);
};

