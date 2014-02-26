#pragma once
class CFileMap
{
public:
    CFileMap(void);
    virtual ~CFileMap(void);

    BOOL Create(LPCTSTR lpszFileName, DWORD dwProtect = PAGE_EXECUTE_READWRITE);
	BOOL Create(DWORD dwSize,DWORD dwProtected = PAGE_EXECUTE_READWRITE);
    void Close();
	HANDLE GetModuleHandle();
    LPVOID GetMapAddress();
    DWORD GetFileSize()const;
private:
    HANDLE m_hFileMap;
    LPVOID m_lpAddr;
    DWORD m_dwFileSize;
};

