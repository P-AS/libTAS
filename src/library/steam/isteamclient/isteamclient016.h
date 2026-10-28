//====== Copyright (c) 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#ifndef LIBTAS_ISTEAMCLIENT016_H_INCL
#define LIBTAS_ISTEAMCLIENT016_H_INCL

#include "isteamclient.h"
#include "steam/steamtypes.h"
#include "steam/isteamuser/isteamuser.h"
#include "steam/isteamuserstats/isteamuserstats.h"
#include "steam/isteamutils.h"
#include "steam/isteamremotestorage/isteamremotestorage.h"
#include "steam/isteamapps.h"
#include "steam/isteamfriends.h"
#include "steam/isteamscreenshots.h"
#include "steam/isteamugc.h"
#include "steam/isteamcontroller.h"
#include "steam/isteammatchmaking.h"
#include "steam/isteamhttp.h"
#include "steam/isteamgameserver.h"
#include "steam/isteamnetworking.h"

#include <stdint.h>
#define STEAMCLIENT_INTERFACE_VERSION_015 "SteamClient015"
#define STEAMCLIENT_INTERFACE_VERSION_016 "SteamClient016"

namespace libtas {

typedef void ISteamAppList;
typedef void ISteamMusic;
typedef void ISteamMusicRemote;
typedef void ISteamHTMLSurface;
typedef void ISteamGameServerStats;

struct ISteamClient016Vtbl
{
    HSteamPipe (STEAM_CALL *CreateSteamPipe)(void *iface);
    bool (STEAM_CALL *BReleaseSteamPipe)(void *iface, HSteamPipe steam_pipe);
    HSteamUser (STEAM_CALL *ConnectToGlobalUser)(void *iface, HSteamPipe steam_pipe);
    HSteamUser (STEAM_CALL *CreateLocalUser)( void *iface, HSteamPipe *phSteamPipe, EAccountType eAccountType );
    void (STEAM_CALL *ReleaseUser)(void *iface, HSteamPipe steam_pipe, HSteamUser steam_user);
    ISteamUser *(STEAM_CALL *GetISteamUser)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamGameServer *(STEAM_CALL *GetISteamGameServer)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    void (STEAM_CALL *SetLocalIPBinding)( void *iface, uint32_t unIP, uint16_t usPort );
    ISteamFriends *(STEAM_CALL *GetISteamFriends)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamUtils *(STEAM_CALL *GetISteamUtils)(void *iface, HSteamPipe steam_pipe, const char *version);
    ISteamMatchmaking *(STEAM_CALL *GetISteamMatchmaking)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamMatchmakingServers *(STEAM_CALL *GetISteamMatchmakingServers)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    void *(STEAM_CALL *GetISteamGenericInterface)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamUserStats *(STEAM_CALL *GetISteamUserStats)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamGameServerStats *(STEAM_CALL *GetISteamGameServerStats)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamApps *(STEAM_CALL *GetISteamApps)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamNetworking *(STEAM_CALL *GetISteamNetworking)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamRemoteStorage *(STEAM_CALL *GetISteamRemoteStorage)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamScreenshots *(STEAM_CALL *GetISteamScreenshots)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    void (STEAM_CALL *RunFrame)(void *iface);
    unsigned int (STEAM_CALL *GetIPCCallCount)(void *iface);
    void (STEAM_CALL *SetWarningMessageHook)(void *iface, void *callback);
    bool (STEAM_CALL *BShutdownIfAllPipesClosed)(void *iface);
    ISteamHTTP *(STEAM_CALL *GetISteamHTTP)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    void *(STEAM_CALL *GetISteamUnifiedMessages)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamController *(STEAM_CALL *GetISteamController)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamUGC *(STEAM_CALL *GetISteamUGC)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamAppList *(STEAM_CALL *GetISteamAppList)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamMusic *(STEAM_CALL *GetISteamMusic)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamMusicRemote *(STEAM_CALL *GetISteamMusicRemote)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamHTMLSurface *(STEAM_CALL *GetISteamHTMLSurface)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    void *Set_SteamAPI_CPostAPIResultInProcess;
    void *Remove_SteamAPI_CPostAPIResultInProcess;
    void *Set_SteamAPI_CCheckCallbackRegisteredInProcess;
};

struct ISteamClient *SteamClient016(void);

}

#endif
