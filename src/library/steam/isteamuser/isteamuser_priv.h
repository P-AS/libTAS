//====== Copyright (c) 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#ifndef LIBTAS_ISTEAMUSER_PRIV_H_INCL
#define LIBTAS_ISTEAMUSER_PRIV_H_INCL

#include "steam/steamtypes.h"

#include <stdint.h>
#include <string>

namespace libtas {

HSteamUser STEAM_CALL ISteamUser_GetHSteamUser( void* iface );
bool STEAM_CALL ISteamUser_BLoggedOn( void* iface );
STEAMID_RET STEAM_CALL ISteamUser_GetSteamID( void* iface STEAMID_RET_ARG_LAST );
int STEAM_CALL ISteamUser_InitiateGameConnection( void* iface, void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, unsigned int unIPServer, uint16_t usPortServer, bool bSecure );
void STEAM_CALL ISteamUser_TerminateGameConnection( void* iface, unsigned int unIPServer, uint16_t usPortServer );
void STEAM_CALL ISteamUser_TrackAppUsageEvent( void* iface, CGameID gameID, int eAppUsageEvent, const char *pchExtraInfo );
bool STEAM_CALL ISteamUser_GetUserDataFolder( void* iface, char *pchBuffer, int cubBuffer );
void STEAM_CALL ISteamUser_StartVoiceRecording( void* iface );
void STEAM_CALL ISteamUser_StopVoiceRecording( void* iface );
EVoiceResult STEAM_CALL ISteamUser_GetAvailableVoice( void* iface, unsigned int *pcbCompressed, unsigned int *pcbUncompressed_Deprecated, unsigned int nUncompressedVoiceDesiredSampleRate_Deprecated);
EVoiceResult STEAM_CALL ISteamUser_GetVoice( void* iface, bool bWantCompressed, void *pDestBuffer, unsigned int cbDestBufferSize, unsigned int *nBytesWritten, bool bWantUncompressed_Deprecated, void *pUncompressedDestBuffer_Deprecated, unsigned int cbUncompressedDestBufferSize_Deprecated, unsigned int *nUncompressBytesWritten_Deprecated, unsigned int nUncompressedVoiceDesiredSampleRate_Deprecated);
EVoiceResult STEAM_CALL ISteamUser_DecompressVoice( void* iface, const void *pCompressed, unsigned int cbCompressed, void *pDestBuffer, unsigned int cbDestBufferSize, unsigned int *nBytesWritten, unsigned int nDesiredSampleRate );
unsigned int STEAM_CALL ISteamUser_GetVoiceOptimalSampleRate( void* iface );
HAuthTicket STEAM_CALL ISteamUser_GetAuthSessionTicket( void* iface, void *pTicket, int cbMaxTicket, unsigned int *pcbTicket );
HAuthTicket STEAM_CALL ISteamUser_GetAuthTicketForWebApi( void* iface, const char *pchIdentity );
EBeginAuthSessionResult STEAM_CALL ISteamUser_BeginAuthSession( void* iface, const void *pAuthTicket, int cbAuthTicket, CSteamID steamID );
void STEAM_CALL ISteamUser_EndAuthSession( void* iface, CSteamID steamID );
void STEAM_CALL ISteamUser_CancelAuthTicket( void* iface, HAuthTicket hAuthTicket );

}

#endif
