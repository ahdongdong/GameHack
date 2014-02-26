#include "stdafx.h"
#include "FileMap.h"
#include <propidl.h>


CFileMap::CFileMap(void): m_hFileMap(NULL), m_lpAddr(NULL), m_dwFileSize(0)
{
}


CFileMap::~CFileMap(void)
{
    Close();
}

BOOL CFileMap::Create(LPCTSTR lpszFileName, DWORD dwProtect)
{
    if(!PathFileExists(lpszFileName))
    {
        return FALSE;
    }

    if(NULL != m_hFileMap)
    {
        return TRUE;
    }

    HANDLE hFile =::CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(INVALID_HANDLE_VALUE == hFile)
    {
        return FALSE;
    }

    m_dwFileSize =::GetFileSize(hFile, 0);
    m_hFileMap =::CreateFileMapping(hFile, NULL, dwProtect, 0, 0, NULL);

    if(NULL == m_hFileMap)
    {
        CloseHandle(hFile);
        return FALSE;
    }

    m_lpAddr =::MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if(NULL == m_lpAddr)
    {
        Close();
        CloseHandle(hFile);
        return FALSE;
    }

    CloseHandle(hFile);
    return TRUE;
}

BOOL CFileMap::Create(DWORD dwSize, DWORD dwProtect /*= PAGE_EXECUTE_READWRITE*/)
{
    if(dwSize <= 0) return FALSE;

    m_hFileMap =::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, dwProtect, 0, dwSize, NULL);

    if(NULL == m_hFileMap)
    {
        return FALSE;
    }

    m_lpAddr =::MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    memset(m_lpAddr, 0, dwSize);

    if(NULL == m_lpAddr)
    {
        Close();
        return FALSE;
    }

    return TRUE;
}

void CFileMap::Close()
{
    if(NULL != m_lpAddr)
    {
        UnmapViewOfFile(m_lpAddr);
        m_lpAddr = NULL;
    }

    if(NULL != m_hFileMap)
    {
        CloseHandle(m_hFileMap);
        m_hFileMap = NULL;
    }

    m_dwFileSize = 0;
}

LPVOID CFileMap::GetMapAddress()
{
    return m_lpAddr;
}

DWORD CFileMap::GetFileSize() const
{
    return m_dwFileSize;
}

HANDLE CFileMap::GetModuleHandle()
{
    return m_hFileMap;
}
