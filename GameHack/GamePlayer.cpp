#include "stdafx.h"
#include "GamePlayer.h"

extern TCHAR g_configPath[MAX_PATH];
CGamePlayer::CGamePlayer()
{
    m_pProHelper = NULL;
}


CGamePlayer::~CGamePlayer( void )
{
}

BOOL CGamePlayer::Init( DWORD dwPID )
{
    if ( !m_configFile.Load( g_configPath ) )
    {
        return FALSE;
    }
    if ( NULL == m_pProHelper )
    {
        m_pProHelper = new CProcessHelper( dwPID );
    }
    
    return m_pProHelper->Open();
}

void CGamePlayer::UnInit()
{
    if ( NULL != m_pProHelper )
    {
        m_pProHelper->Close();
        delete m_pProHelper;
        m_pProHelper = NULL;
    }
    
    m_configFile.UnLoad();
}

DWORD CGamePlayer::GetGameBase() const
{
    DWORD dwGameBase = m_configFile.GetGameBase() + m_configFile.GetGameModule();
    DWORD dwBuf = 0;
    m_pProHelper->ReadMemory( ( LPCVOID )dwGameBase, &dwBuf, sizeof( dwBuf ) );
    return dwBuf;
}

CString CGamePlayer::GetCurAxis() const
{
    DWORD dwAxisX = m_configFile.GetAxisX() + GetGameBase();
    DWORD dwBufX = 0;
    m_pProHelper->ReadMemory( ( LPCVOID )dwAxisX, &dwBufX, sizeof( dwBufX ) );
    
    DWORD dwAxisY = m_configFile.GetAxisY() + GetGameBase();
    DWORD dwBufY = 0;
    m_pProHelper->ReadMemory( ( LPCVOID )dwAxisY, &dwBufY, sizeof( dwBufY ) );
    
    CString sRet;
    sRet.Format( _T( "当前坐标：%u，%u" ), dwBufX, dwBufY );
    return sRet;
}

CString CGamePlayer::GetPlayerName() const
{
    DWORD dwAddr = m_configFile.GetPlayerName() + m_configFile.GetGameModule();
    BYTE sName[12] = {0};
    m_pProHelper->ReadMemory( ( LPCVOID )dwAddr, sName, sizeof( sName ) );
    CString sPlayerName;
	sPlayerName.Format( _T( "人物名称：%s" ) , sName );
    return sPlayerName;
}
