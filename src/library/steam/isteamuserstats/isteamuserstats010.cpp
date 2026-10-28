//====== Copyright (c) 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#include "isteamuserstats010.h"
#include "isteamuserstats_priv.h"

namespace libtas {

static const struct ISteamUserStats010Vtbl ISteamUserStats010_vtbl = {
    
    ISteamUserStats_RequestCurrentStats,
    STEAM_OVERLOAD_PAIR(ISteamUserStats_GetStatInt32, ISteamUserStats_GetStatFloat),
    STEAM_OVERLOAD_PAIR(ISteamUserStats_SetStatInt32, ISteamUserStats_SetStatFloat),
    ISteamUserStats_UpdateAvgRateStat,
    ISteamUserStats_GetAchievement,
    ISteamUserStats_SetAchievement,
    ISteamUserStats_ClearAchievement,
    ISteamUserStats_GetAchievementAndUnlockTime,
    ISteamUserStats_StoreStats,
    ISteamUserStats_GetAchievementIcon,
    ISteamUserStats_GetAchievementDisplayAttribute,
    ISteamUserStats_IndicateAchievementProgress,
    ISteamUserStats_RequestUserStats,
    STEAM_OVERLOAD_PAIR(ISteamUserStats_GetUserStatInt32, ISteamUserStats_GetUserStatFloat),
    ISteamUserStats_GetUserAchievement,
    ISteamUserStats_GetUserAchievementAndUnlockTime,
    ISteamUserStats_ResetAllStats,
    ISteamUserStats_FindOrCreateLeaderboard,
    ISteamUserStats_FindLeaderboard,
    ISteamUserStats_GetLeaderboardName,
    ISteamUserStats_GetLeaderboardEntryCount,
    ISteamUserStats_GetLeaderboardSortMethod,
    ISteamUserStats_GetLeaderboardDisplayType,
    ISteamUserStats_DownloadLeaderboardEntries,
    ISteamUserStats_DownloadLeaderboardEntriesForUsers,
    ISteamUserStats_GetDownloadedLeaderboardEntry,
    ISteamUserStats_UploadLeaderboardScore,
    ISteamUserStats_AttachLeaderboardUGC,
    ISteamUserStats_GetNumberOfCurrentPlayers,
    ISteamUserStats_RequestGlobalAchievementPercentages,
    ISteamUserStats_GetMostAchievedAchievementInfo,
    ISteamUserStats_GetNextMostAchievedAchievementInfo,
    ISteamUserStats_GetAchievementAchievedPercent,
    ISteamUserStats_RequestGlobalStats,
    STEAM_OVERLOAD_PAIR(ISteamUserStats_GetGlobalStatInt64, ISteamUserStats_GetGlobalStatDouble),
    STEAM_OVERLOAD_PAIR(ISteamUserStats_GetGlobalStatHistoryInt64, ISteamUserStats_GetGlobalStatHistoryDouble),
};

struct ISteamUserStats *SteamUserStats010(void)
{
    static struct ISteamUserStats impl;

    impl.vtbl.v010 = &ISteamUserStats010_vtbl;

    return &impl;
}

}
