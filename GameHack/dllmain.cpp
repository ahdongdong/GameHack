// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "DlgMain.h"
#include "GamePlayer.h"
#include "WinHook.h"

#define DLL_EXPORT_FUNC extern "C" __declspec(dllexport)

extern CWindowsHook* g_pHook;
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                     )
{
    switch(ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls(hModule);
            g_pHook = new CWinHook(hModule);
            break;
        }

        case DLL_THREAD_ATTACH:
        {
            break;
        }

        case DLL_THREAD_DETACH:
        {
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            g_pHook->UnInstall();

            if(NULL != g_pHook)
            {
                delete g_pHook;
                g_pHook = NULL;
            }

            break;
        }

        default:
            break;
    }

    return TRUE;
}

DLL_EXPORT_FUNC BOOL InstallWindowHook(DWORD dwPID)
{
    return g_pHook->Install(WH_KEYBOARD, dwPID);
}

DLL_EXPORT_FUNC void UnInstallWindowHook()
{
    g_pHook->UnInstall();
}
