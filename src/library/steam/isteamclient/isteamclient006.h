//====== Copyright (c) 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#ifndef LIBTAS_ISTEAMCLIENT006_H_INCL
#define LIBTAS_ISTEAMCLIENT006_H_INCL

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
#define STEAMCLIENT_INTERFACE_VERSION_006 "SteamClient006"

namespace libtas {

struct ISteamClient006Vtbl
{
    HSteamPipe (STEAM_CALL *CreateSteamPipe)(void *iface);
    bool (STEAM_CALL *BReleaseSteamPipe)(void *iface, HSteamPipe steam_pipe);
    void *CreateGlobalUser;
    HSteamUser (STEAM_CALL *ConnectToGlobalUser)(void *iface, HSteamPipe steam_pipe);
    HSteamUser (STEAM_CALL *CreateLocalUser)( void *iface, HSteamPipe *phSteamPipe, EAccountType eAccountType );
    void (STEAM_CALL *ReleaseUser)(void *iface, HSteamPipe steam_pipe, HSteamUser steam_user);
    ISteamUser *(STEAM_CALL *GetISteamUser)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    void *GetIVAC;
    ISteamGameServer *(STEAM_CALL *GetISteamGameServer)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    void *SetLocalIPBinding;
    void *GetUniverseName;
    ISteamFriends *(STEAM_CALL *GetISteamFriends)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamUtils *(STEAM_CALL *GetISteamUtils)(void *iface, HSteamPipe steam_pipe, const char *version);
    void *GetISteamBilling;
    ISteamMatchmaking *(STEAM_CALL *GetISteamMatchmaking)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    ISteamApps *(STEAM_CALL *GetISteamApps)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    void *GetISteamContentServer;
    void *GetISteamMasterServerUpdater;
    ISteamMatchmakingServers *(STEAM_CALL *GetISteamMatchmakingServers)(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
    void (STEAM_CALL *RunFrame)(void *iface);
    unsigned int (STEAM_CALL *GetIPCCallCount)(void *iface);
};

struct ISteamClient *SteamClient006(void);

}

#endif
