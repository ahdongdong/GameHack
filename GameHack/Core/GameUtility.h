#pragma once

DWORD GetMTIDFromPID(DWORD dwProcessID);
HWND GetHwndFromPID(DWORD dwProcessID);
BOOL EnablePrivilege(LPCTSTR PrivilegeName);
DWORD GetPidFromName(LPCTSTR sProName);