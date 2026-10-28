//====== Copyright (c) 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#ifndef LIBTAS_ISTEAMCLIENT_PRIV_H_INCL
#define LIBTAS_ISTEAMCLIENT_PRIV_H_INCL

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
#include "steam/isteaminput.h"
#include "steam/isteamgameserver.h"
#include "steam/isteamnetworking.h"

#include <stdint.h>
#include <string>

namespace libtas {

typedef void ISteamAppList;
typedef void ISteamMusic;
typedef void ISteamMusicRemote;
typedef void ISteamHTMLSurface;
typedef void ISteamInventory;
typedef void ISteamVideo;
typedef void ISteamParentalSettings;
typedef void ISteamGameServerStats;
typedef void ISteamGameSearch;
typedef void ISteamParties;
typedef void ISteamRemotePlay;

HSteamPipe STEAM_CALL ISteamClient_CreateSteamPipe(void *iface);
bool STEAM_CALL ISteamClient_BReleaseSteamPipe(void *iface, HSteamPipe steam_pipe);
HSteamUser STEAM_CALL ISteamClient_ConnectToGlobalUser(void *iface, HSteamPipe steam_pipe);
HSteamUser STEAM_CALL ISteamClient_CreateLocalUser( void *iface, HSteamPipe *phSteamPipe, EAccountType eAccountType );
void STEAM_CALL ISteamClient_ReleaseUser(void *iface, HSteamPipe steam_pipe, HSteamUser steam_user);
ISteamAppList *STEAM_CALL ISteamClient_GetISteamAppList(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamApps *STEAM_CALL ISteamClient_GetISteamApps(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamClient *STEAM_CALL ISteamClient_GetISteamClient(void *iface, HSteamPipe steam_pipe, const char *version);
ISteamController *STEAM_CALL ISteamClient_GetISteamController(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamFriends *STEAM_CALL ISteamClient_GetISteamFriends(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamGameServer *STEAM_CALL ISteamClient_GetISteamGameServer(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamGameServerStats *STEAM_CALL ISteamClient_GetISteamGameServerStats(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
void *STEAM_CALL ISteamClient_GetISteamGenericInterface(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamHTMLSurface *STEAM_CALL ISteamClient_GetISteamHTMLSurface(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamHTTP *STEAM_CALL ISteamClient_GetISteamHTTP(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamInventory *STEAM_CALL ISteamClient_GetISteamInventory(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamMatchmaking *STEAM_CALL ISteamClient_GetISteamMatchmaking(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamMatchmakingServers *STEAM_CALL ISteamClient_GetISteamMatchmakingServers(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamMusic *STEAM_CALL ISteamClient_GetISteamMusic(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamMusicRemote *STEAM_CALL ISteamClient_GetISteamMusicRemote(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamNetworking *STEAM_CALL ISteamClient_GetISteamNetworking(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamParentalSettings *STEAM_CALL ISteamClient_GetISteamParentalSettings(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamRemoteStorage *STEAM_CALL ISteamClient_GetISteamRemoteStorage(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamScreenshots *STEAM_CALL ISteamClient_GetISteamScreenshots(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamUGC *STEAM_CALL ISteamClient_GetISteamUGC(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
void *STEAM_CALL ISteamClient_GetISteamUnifiedMessages(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamUser *STEAM_CALL ISteamClient_GetISteamUser(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamUserStats *STEAM_CALL ISteamClient_GetISteamUserStats(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamUtils *STEAM_CALL ISteamClient_GetISteamUtils(void *iface, HSteamPipe steam_pipe, const char *version);
ISteamVideo *STEAM_CALL ISteamClient_GetISteamVideo(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
void STEAM_CALL ISteamClient_SetWarningMessageHook(void *iface, void *callback);
void STEAM_CALL ISteamClient_SetLocalIPBinding( void *iface, uint32_t unIP, uint16_t usPort );
void STEAM_CALL ISteamClient_RunFrame(void *iface);
unsigned int STEAM_CALL ISteamClient_GetIPCCallCount(void *iface);
bool STEAM_CALL ISteamClient_BShutdownIfAllPipesClosed(void *iface);
void STEAM_CALL ISteamClient_DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess( void *iface, void (*)() );
void STEAM_CALL ISteamClient_DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess( void *iface, void (*)() );
void STEAM_CALL ISteamClient_Set_SteamAPI_CCheckCallbackRegisteredInProcess( void *iface, SteamAPI_CheckCallbackRegistered_t func );
ISteamGameSearch *STEAM_CALL ISteamClient_GetISteamGameSearch(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamInput *STEAM_CALL ISteamClient_GetISteamInput(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamParties *STEAM_CALL ISteamClient_GetISteamParties(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);
ISteamRemotePlay *STEAM_CALL ISteamClient_GetISteamRemotePlay(void *iface, HSteamUser steam_user, HSteamPipe steam_pipe, const char *version);

}

#endif
