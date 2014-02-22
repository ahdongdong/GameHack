#pragma once

class CWindowsHook
{
public:
    CWindowsHook(HINSTANCE hModule);
    virtual ~CWindowsHook(void);

    BOOL Install(int iHook, DWORD dwPID);
    BOOL UnInstall();
protected:
    DWORD GetPID()const;
    virtual BOOL HookProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK _HookProc(int nCode, WPARAM wParam, LPARAM lParam);
    HINSTANCE m_hModule;
private:
    static HHOOK m_hHook;
    static CWindowsHook* m_pThis;
};

