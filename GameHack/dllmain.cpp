// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "DlgMain.h"
#include "GamePlayer.h"

#pragma data_seg(".GameHack")
HHOOK g_hKeyboardHook = NULL;
CDlgMain* g_dlgMain = NULL;
DWORD g_dwPID = -1;
HWND g_hParent = NULL;
HMODULE g_hDll = NULL;
CGamePlayer g_gamePlayer;
//CString g_configPath;
TCHAR g_configPath[MAX_PATH] = {0};
#pragma data_seg()
#pragma comment(linker, "/Section:.GameHack,RWS")
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch ( ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls( hModule );
            g_hDll = hModule;
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
            if ( NULL != g_dlgMain )
            {
                delete g_dlgMain;
                g_dlgMain = NULL;
            }
            break;
        }
        default:
            break;
    }
    return TRUE;
}

