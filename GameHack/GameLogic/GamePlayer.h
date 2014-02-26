#pragma once

class CProcessHelper;
class CConfigFile;

class CGamePlayer
{
public:
    CGamePlayer(CProcessHelper* pProHlp, CConfigFile* pCfgHlp);
    virtual ~CGamePlayer(void);

    CString GetCurAxis()const;
    CString GetPlayerName()const;
protected:
    DWORD GetGameBase()const;
private:
    CProcessHelper* m_pProHelper;
    CConfigFile* m_pConfigFile;
};

