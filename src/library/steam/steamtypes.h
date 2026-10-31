//====== Copyright (c) 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#ifndef LIBTAS_STEAMTYPES_H_INCL
#define LIBTAS_STEAMTYPES_H_INCL

#include <stdint.h>

namespace libtas {

/* Calling convention of the interface functions that we put in C vtables.
 * Windows games (built with MSVC) call interface methods with `thiscall` on
 * i386, where `this` is passed in ecx and the callee pops the arguments. */
#if defined(_WIN32) && defined(__i386__)
#define STEAM_CALL __attribute__((thiscall))
#else
#define STEAM_CALL
#endif

/* MSVC puts overloaded virtual functions in the vtable in the reverse order
 * of their declaration, so swap them for Windows games. Both functions take
 * the same arguments except for the type of the overloaded one. */
#ifdef _WIN32
#define STEAM_OVERLOAD_PAIR(A, B) reinterpret_cast<decltype(&A)>(&B), reinterpret_cast<decltype(&B)>(&A)
#else
#define STEAM_OVERLOAD_PAIR(A, B) A, B
#endif

/* MSVC member functions return class types (CSteamID is a class in the
 * Steamworks SDK) through a hidden pointer passed right after `this`, and
 * return that pointer. Functions returning a CSteamID are declared with
 * STEAMID_RET as return type, take STEAMID_RET_ARG as first argument after
 * `this` (or STEAMID_RET_ARG_ONLY if they have no other argument, or
 * STEAMID_RET_ARG_LAST after the iface argument of C vtable functions), and
 * return with STEAMID_RETURN(). */
#ifdef _WIN32
#define STEAMID_RET CSteamID*
#define STEAMID_RET_ARG CSteamID *steamid_ret,
#define STEAMID_RET_ARG_ONLY CSteamID *steamid_ret
#define STEAMID_RET_ARG_LAST , CSteamID *steamid_ret
#define STEAMID_RETURN(V) do { *steamid_ret = (V); return steamid_ret; } while (0)
#else
#define STEAMID_RET CSteamID
#define STEAMID_RET_ARG
#define STEAMID_RET_ARG_ONLY
#define STEAMID_RET_ARG_LAST
#define STEAMID_RETURN(V) return (V)
#endif

#define DUMMY_STEAM_IMPL(NAME, ARGS, RETTYPE, RET) \
RETTYPE NAME ARGS {\
    LOGTRACE_SIMPLE(LCF_STEAM);\
    return RET;\
}

typedef int HSteamUser;
typedef int HSteamPipe;
typedef unsigned int HAuthTicket;
typedef uint32_t ScreenshotHandle;

typedef uint64_t CSteamID;
typedef uint64_t CGameID;

typedef unsigned int AppId_t;
typedef uint64_t SteamAPICall_t;
typedef uint32_t DepotId_t;
typedef void CallbackMsg_t;

typedef uint64_t PublishedFileId_t; // not sure
typedef uint64_t SteamParamStringArray_t; // not sure

typedef uint64_t UGCFileWriteStreamHandle_t;
typedef uint64_t UGCQueryHandle_t;
typedef uint64_t UGCUpdateHandle_t;
typedef uint32_t AccountID_t;
typedef uint32_t PartnerId_t;
typedef void SteamUGCDetails_t;
typedef void FriendGameInfo_t;
typedef void LeaderboardEntry_t;
typedef void P2PSessionState_t;

typedef int EResult;
typedef int EVoiceResult;
typedef int EBeginAuthSessionResult;
typedef int EUserHasLicenseForAppResult;
typedef int ELeaderboardSortMethod;
typedef int ELeaderboardDisplayType;
typedef int ELeaderboardDataRequest;
typedef int ESteamAPICallFailure;
typedef int EGamepadTextInputMode;
typedef int EGamepadTextInputLineMode;
typedef int EUniverse;
typedef int ENotificationPosition;
typedef int ERemoteStoragePlatform;
typedef int EAccountType;
typedef int EPersonaState;
typedef int EFriendRelationship;
typedef int16_t FriendsGroupID_t;
typedef int EOverlayToStoreFlag;
typedef int EVRScreenshotType;

typedef int EUGCQuery;
typedef int EUGCMatchingUGCType;
typedef int EUserUGCList;
typedef int EUserUGCListSortOrder;
typedef int EItemStatistic;
typedef int EItemPreviewType;
typedef int EItemUpdateStatus;
typedef int ERemoteStoragePublishedFileVisibility;
typedef int EWorkshopFileType;
typedef int ELeaderboardUploadScoreMethod;
typedef int EWorkshopVote;
typedef int EWorkshopFileAction;
typedef int EWorkshopEnumerationType;
typedef int EWorkshopVideoProvider;
typedef int EUGCReadAction;

typedef int ELobbyType;
typedef int ELobbyComparison;
typedef int ELobbyDistanceFilter;
typedef int EGCResults;
typedef int HTTPRequestHandle;
typedef int HTTPCookieContainerHandle;
typedef int EHTTPMethod;
typedef int EHTTPStatusCode;
typedef int EServerMode;

typedef int EP2PSessionError;
typedef int EP2PSend;
typedef uint32_t SNetSocket_t;
typedef uint32_t SNetListenSocket_t;
typedef int ESNetSocketState;
typedef int ESNetSocketConnectionType;

typedef void* HServerListRequest;
typedef int HServerQuery;

typedef uint8_t EChatEntryType;
typedef void MatchMakingKeyValuePair_t;
typedef void ISteamMatchmakingServerListResponse;
typedef void ISteamMatchmakingPingResponse;
typedef void ISteamMatchmakingPlayersResponse;
typedef void ISteamMatchmakingRulesResponse;
typedef void gameserveritem_t;
typedef int EMatchMakingServerResponse;

typedef void (*SteamAPIWarningMessageHook_t)(int, const char *);
typedef uint32_t ( *SteamAPI_CheckCallbackRegistered_t )( int iCallbackNum );

typedef uint64_t UGCHandle_t;
typedef uint64_t PublishedFileUpdateHandle_t;
typedef uint64_t PublishedFileId_t;

typedef void SteamRelayNetworkStatus_t;
typedef void SteamNetworkingMessage_t;
typedef int SteamNetworkPingLocation_t;
typedef void* FSteamNetworkingSocketsDebugOutput;
typedef int SteamNetworkingIPAddr;
typedef int SteamNetworkingIdentity;
typedef int SteamNetworkingConfigValue_t;


typedef int ESteamNetworkingAvailability;
typedef int SteamNetworkingPOPID;
typedef double SteamNetworkingMicroseconds;
typedef int ESteamNetworkingSocketsDebugOutputType;
typedef int ESteamNetworkingFakeIPType;
typedef int ESteamNetworkingConfigValue;
typedef int ESteamNetworkingConfigScope;
typedef int ESteamNetworkingConfigDataType;
typedef int ESteamNetworkingGetConfigValueResult;
typedef int ERemoteStorageLocalFileChange;
typedef int ERemoteStorageFilePathType;

typedef int HSteamListenSocket;
typedef int HSteamNetConnection;
typedef int SteamNetConnectionInfo_t;
typedef int SteamNetConnectionRealTimeLaneStatus_t;
typedef int SteamDatagramRelayAuthTicket;
typedef int HSteamNetPollGroup;
typedef int SteamNetAuthenticationStatus_t;
typedef int SteamDatagramHostedAddress;

typedef int SteamNetConnectionRealTimeStatus_t;
typedef int SteamDatagramGameCoordinatorServerLogin;
typedef int ISteamNetworkingConnectionSignaling;
typedef int ISteamNetworkingSignalingRecvContext;
typedef int SteamNetworkingErrMsg;
typedef int SteamNetworkingFakeIPResult_t;
typedef int ISteamNetworkingFakeUDPPort;
typedef int ESteamNetworkingConnectionState;

typedef uint64_t InputHandle_t;
typedef uint64_t InputActionSetHandle_t;
typedef uint64_t InputDigitalActionHandle_t;
typedef uint64_t InputAnalogActionHandle_t;
typedef void* SteamInputActionEventCallbackPointer;
typedef void ScePadTriggerEffectParam;
typedef int InputMotionData_t;
typedef int InputDigitalActionData_t;
typedef int InputAnalogActionData_t;

typedef uint64_t SteamLeaderboard_t;
typedef uint64_t SteamLeaderboardEntries_t;
typedef uint64_t UGCHandle_t;

#ifdef _WIN32
/* Callback structs use 8-byte packing on Windows */
struct UserStatsReceived_t
{
    uint64_t m_nGameID;
    EResult m_eResult;
    CSteamID m_steamIDUser;
};
#else
struct __attribute__((packed, aligned(1))) UserStatsReceived_t
{
    uint64_t m_nGameID __attribute__((aligned(4)));
    EResult m_eResult __attribute__((aligned(4)));
    CSteamID m_steamIDUser __attribute__((aligned(4)));
};
#endif

#ifdef _WIN32
struct UserStatsStored_t
{
    uint64_t m_nGameID;
    EResult m_eResult;
};

struct UserAchievementStored_t
{
    uint64_t m_nGameID;
    bool m_bGroupAchievement;
    char m_rgchAchievementName[128];
    uint32_t m_nCurProgress;
    uint32_t m_nMaxProgress;
};
#else
struct __attribute__((packed, aligned(1))) UserStatsStored_t
{
    uint64_t m_nGameID __attribute__((aligned(4)));
    EResult m_eResult __attribute__((aligned(4)));
};

struct __attribute__((packed, aligned(1))) UserAchievementStored_t
{
    uint64_t m_nGameID __attribute__((aligned(4)));
    bool m_bGroupAchievement;
    char m_rgchAchievementName[128];
    uint32_t m_nCurProgress;
    uint32_t m_nMaxProgress;
};
#endif

}

#endif
