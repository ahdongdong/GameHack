
#pragma once
#include <Windows.h>

class CIniReader
{
public:
    CIniReader(LPCTSTR lpszIniPath);
    virtual ~CIniReader(void);

    LPCTSTR GetIniFilePath() const;

    DWORD ReadDword(LPCTSTR Section, LPCTSTR key) const;
    DOUBLE ReadDouble(LPCTSTR Section, LPCTSTR key);
    DWORD ReadString(LPCTSTR Section, LPCTSTR key, TCHAR* Buf) const;
    DWORD ReadStringToHex(LPCTSTR Section, LPCTSTR key);

    BOOL WriteDword(LPCTSTR Section, LPCTSTR key, DWORD Value);
    BOOL WriteDouble(LPCTSTR Section, LPCTSTR key, double Value);
    BOOL WriteString(LPCTSTR Section, LPCTSTR key, LPCTSTR Value);
private:
    TCHAR m_IniPath[MAX_PATH];
    TCHAR m_result[MAX_PATH];
};
