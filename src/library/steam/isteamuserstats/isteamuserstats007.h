//====== Copyright ))(c) 1996-2007, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#ifndef LIBTAS_ISTEAMUSERSTATS007_H_INCL
#define LIBTAS_ISTEAMUSERSTATS007_H_INCL

#include "isteamuserstats.h"

#include <stdint.h>
#define STEAMUSERSTATS_INTERFACE_VERSION_007 "STEAMUSERSTATS_INTERFACE_VERSION007"

namespace libtas {

struct ISteamUserStats007Vtbl
{
    bool (STEAM_CALL *RequestCurrentStats)( void* iface );
    bool (STEAM_CALL *GetStatInt32)( void* iface, const char *pchName, int *pData );
    bool (STEAM_CALL *GetStatFloat)( void* iface, const char *pchName, float *pData );
    bool (STEAM_CALL *SetStatInt32)( void* iface, const char *pchName, int nData );
    bool (STEAM_CALL *SetStatFloat)( void* iface, const char *pchName, float fData );
    bool (STEAM_CALL *UpdateAvgRateStat)( void* iface, const char *pchName, float flCountThisSession, double dSessionLength );
    bool (STEAM_CALL *GetAchievement)( void* iface, const char *pchName, bool *pbAchieved );
    bool (STEAM_CALL *SetAchievement)( void* iface, const char *pchName );
    bool (STEAM_CALL *ClearAchievement)( void* iface, const char *pchName );
    bool (STEAM_CALL *GetAchievementAndUnlockTime)( void* iface, const char *pchName, bool *pbAchieved, unsigned int *punUnlockTime );
    bool (STEAM_CALL *StoreStats)( void* iface );
    int (STEAM_CALL *GetAchievementIcon)( void* iface, const char *pchName );
    const char *(STEAM_CALL *GetAchievementDisplayAttribute)( void* iface, const char *pchName, const char *pchKey );
    bool (STEAM_CALL *IndicateAchievementProgress)( void* iface, const char *pchName, unsigned int nCurProgress, unsigned int nMaxProgress );
    SteamAPICall_t (STEAM_CALL *RequestUserStats)( void* iface, CSteamID steamIDUser );
    bool (STEAM_CALL *GetUserStatInt32)( void* iface, CSteamID steamIDUser, const char *pchName, int *pData );
    bool (STEAM_CALL *GetUserStatFloat)( void* iface, CSteamID steamIDUser, const char *pchName, float *pData );
    bool (STEAM_CALL *GetUserAchievement)( void* iface, CSteamID steamIDUser, const char *pchName, bool *pbAchieved );
    bool (STEAM_CALL *GetUserAchievementAndUnlockTime)( void* iface, CSteamID steamIDUser, const char *pchName, bool *pbAchieved, unsigned int *punUnlockTime );
    bool (STEAM_CALL *ResetAllStats)( void* iface, bool bAchievementsToo );
    SteamAPICall_t (STEAM_CALL *FindOrCreateLeaderboard)( void* iface, const char *pchLeaderboardName, ELeaderboardSortMethod eLeaderboardSortMethod, ELeaderboardDisplayType eLeaderboardDisplayType );
    SteamAPICall_t (STEAM_CALL *FindLeaderboard)( void* iface, const char *pchLeaderboardName );
    const char *(STEAM_CALL *GetLeaderboardName)( void* iface, SteamLeaderboard_t hSteamLeaderboard );
    int (STEAM_CALL *GetLeaderboardEntryCount)( void* iface, SteamLeaderboard_t hSteamLeaderboard );
    ELeaderboardSortMethod (STEAM_CALL *GetLeaderboardSortMethod)( void* iface, SteamLeaderboard_t hSteamLeaderboard );
    ELeaderboardDisplayType (STEAM_CALL *GetLeaderboardDisplayType)( void* iface, SteamLeaderboard_t hSteamLeaderboard );
    SteamAPICall_t (STEAM_CALL *DownloadLeaderboardEntries)( void* iface, SteamLeaderboard_t hSteamLeaderboard, ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart, int nRangeEnd );
    bool (STEAM_CALL *GetDownloadedLeaderboardEntry)( void* iface, SteamLeaderboardEntries_t hSteamLeaderboardEntries, int index, LeaderboardEntry_t *pLeaderboardEntry, int *pDetails, int cDetailsMax );
    SteamAPICall_t (STEAM_CALL *UploadLeaderboardScore)( void* iface, SteamLeaderboard_t hSteamLeaderboard, ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int nScore, const int *pScoreDetails, int cScoreDetailsCount );
    SteamAPICall_t (STEAM_CALL *GetNumberOfCurrentPlayers)( void* iface );
};

struct ISteamUserStats *SteamUserStats007(void);

}

#endif
