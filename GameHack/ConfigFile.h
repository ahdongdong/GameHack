#pragma once
#include "IniReader.h"
class CConfigFile
{
public:
    CConfigFile(void);
    virtual ~CConfigFile(void);

    BOOL Load(LPCTSTR lpszPath);
    void UnLoad();
    DWORD GetGameBase()const;
    DWORD GetGameModule()const;
    DWORD GetAxisX()const;
    DWORD GetAxisY()const;
    DWORD GetPlayerName()const;
private:
    CIniReader* m_pIniReader;
};

