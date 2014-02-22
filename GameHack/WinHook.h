#pragma once
#include "WindowsHook.h"
#include "DlgMain.h"

class CWinHook : public CWindowsHook
{
public:
    CWinHook(HINSTANCE hModule);
    virtual ~CWinHook(void);

    virtual BOOL HookProc(int nCode, WPARAM wParam, LPARAM lParam);
private:
    CDlgMain* m_pDlgMain;
};

