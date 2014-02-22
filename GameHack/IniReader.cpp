#include "stdafx.h"
#include "IniReader.h"
#include <Shlwapi.h>
#include <atldef.h>
#include <tchar.h>

CIniReader::CIniReader(LPCTSTR lpszIniPath)
{
    ATLASSERT(NULL != lpszIniPath);
    ZeroMemory(m_IniPath, MAX_PATH);
    ZeroMemory(m_result, MAX_PATH);
    _tcscpy_s(m_IniPath, MAX_PATH, lpszIniPath);
}

CIniReader::~CIniReader(void)
{
}

BOOL CIniReader::WriteDword(LPCTSTR Section, LPCTSTR key, DWORD Value)
{
    _stprintf_s(m_result, MAX_PATH, _T("%u"), Value);
    return WritePrivateProfileString(Section, key, m_result, m_IniPath);
}

BOOL CIniReader::WriteDouble(LPCTSTR Section, LPCTSTR key, double Value)
{
    _stprintf_s(m_result, MAX_PATH, _T("%f"), Value);
    return WritePrivateProfileString(Section, key, m_result, m_IniPath);
}

BOOL CIniReader::WriteString(LPCTSTR Section, LPCTSTR key, LPCTSTR Value)
{
    return WritePrivateProfileString(Section, key, Value, m_IniPath);
}

DWORD CIniReader::ReadDword(LPCTSTR Section, LPCTSTR key) const
{
    return GetPrivateProfileInt(Section, key, 0, m_IniPath);
}

double CIniReader::ReadDouble(LPCTSTR Section, LPCTSTR key)
{
    ReadString(Section, key, m_result);
    return _tstof(m_result);
}

DWORD CIniReader::ReadString(LPCTSTR Section, LPCTSTR key, TCHAR* Buf) const
{
    DWORD dwReaded = GetPrivateProfileString(Section, key, 0, Buf, MAX_PATH, m_IniPath);
    return dwReaded + 1;    // +terminating null character
}

LPCTSTR CIniReader::GetIniFilePath() const
{
    return m_IniPath;
}

DWORD CIniReader::ReadStringToHex(LPCTSTR Section, LPCTSTR key)
{
    LONGLONG lRet = 0;
    TCHAR sRet[MAX_PATH] = {0};
    GetPrivateProfileString(Section, key, 0, sRet, MAX_PATH, m_IniPath);
    StrToInt64Ex(sRet, STIF_SUPPORT_HEX, &lRet);
    return (DWORD)lRet;
}
