/********************************************************************
	created:	2013/08/23
	created:	23:8:2013   0:26
	author:		redcode
	purpose:	键盘钩子，呼出外挂窗口
*********************************************************************/
#pragma once
#include <Windows.h>

#define DLL_EXPORT_FUNC extern "C" __declspec(dllexport)

LRESULT CALLBACK KeyboardProc( int nCode, WPARAM wParam, LPARAM lParam );

DLL_EXPORT_FUNC BOOL InstallWindowHook( DWORD dwPID );

DLL_EXPORT_FUNC void UnInstallWindowHook();

DLL_EXPORT_FUNC void SetConfigPath( LPCTSTR lpszPath );
