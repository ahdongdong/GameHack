#pragma once
#include <atlstr.h>

class CGameHelper
{
    typedef BOOL (*InstallWindowHook)(DWORD dwPID);
    typedef void (*UnInstallWindowHook)();
public:
    CGameHelper(void);
    virtual ~CGameHelper(void);

    BOOL Start(CString sProName);
    BOOL Stop();
protected:
    BOOL Init();
    void UnInit();
    BOOL InstallHook(DWORD dwPID);
    BOOL UnInstallHook();
    CString GetGameHackDllPath();

private:
    HMODULE m_hGameHackDll;
    InstallWindowHook m_InstallWH;
    UnInstallWindowHook m_UnInstallWH;
};

