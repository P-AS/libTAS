//====== Copyright )(c) 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#ifndef LIBTAS_ISTEAMUSER023_H_INCL
#define LIBTAS_ISTEAMUSER023_H_INCL

#include "isteamuser.h"

#include <stdint.h>
#define STEAMUSER_INTERFACE_VERSION_023 "SteamUser023"

namespace libtas {

struct ISteamUser023Vtbl
{
    HSteamUser (STEAM_CALL *GetHSteamUser)( void* iface );
    bool (STEAM_CALL *BLoggedOn)( void* iface );
    STEAMID_RET (STEAM_CALL *GetSteamID)( void* iface STEAMID_RET_ARG_LAST );
    int (STEAM_CALL *InitiateGameConnection)( void* iface, void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, unsigned int unIPServer, uint16_t usPortServer, bool bSecure );
    void (STEAM_CALL *TerminateGameConnection)( void* iface, unsigned int unIPServer, uint16_t usPortServer );
    void (STEAM_CALL *TrackAppUsageEvent)( void* iface, CGameID gameID, int eAppUsageEvent, const char *pchExtraInfo );
    bool (STEAM_CALL *GetUserDataFolder)( void* iface, char *pchBuffer, int cubBuffer );
    void (STEAM_CALL *StartVoiceRecording)( void* iface );
    void (STEAM_CALL *StopVoiceRecording)( void* iface );
    EVoiceResult (STEAM_CALL *GetAvailableVoice)( void* iface, unsigned int *pcbCompressed, unsigned int *pcbUncompressed_Deprecated, unsigned int nUncompressedVoiceDesiredSampleRate_Deprecated);
    EVoiceResult (STEAM_CALL *GetVoice)( void* iface, bool bWantCompressed, void *pDestBuffer, unsigned int cbDestBufferSize, unsigned int *nBytesWritten, bool bWantUncompressed_Deprecated, void *pUncompressedDestBuffer_Deprecated, unsigned int cbUncompressedDestBufferSize_Deprecated, unsigned int *nUncompressBytesWritten_Deprecated, unsigned int nUncompressedVoiceDesiredSampleRate_Deprecated);
    EVoiceResult (STEAM_CALL *DecompressVoice)( void* iface, const void *pCompressed, unsigned int cbCompressed, void *pDestBuffer, unsigned int cbDestBufferSize, unsigned int *nBytesWritten, unsigned int nDesiredSampleRate );
    unsigned int (STEAM_CALL *GetVoiceOptimalSampleRate)( void* iface );
    HAuthTicket (STEAM_CALL *GetAuthSessionTicket)( void* iface, void *pTicket, int cbMaxTicket, unsigned int *pcbTicket );
    HAuthTicket (STEAM_CALL *GetAuthTicketForWebApi)( void* iface, const char *pchIdentity );
    EBeginAuthSessionResult (STEAM_CALL *BeginAuthSession)( void* iface, const void *pAuthTicket, int cbAuthTicket, CSteamID steamID );
    void (STEAM_CALL *EndAuthSession)( void* iface, CSteamID steamID );
    void (STEAM_CALL *CancelAuthTicket)( void* iface, HAuthTicket hAuthTicket );
	// EUserHasLicenseForAppResult (*UserHasLicenseForApp)( CSteamID steamID, AppId_t appID );
	// bool (*BIsBehindNAT)();
	// void (*AdvertiseGame)( CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer );
	// SteamAPICall_t (*RequestEncryptedAppTicket)( void *pDataToInclude, int cbDataToInclude );
	// bool (*GetEncryptedAppTicket)( void *pTicket, int cbMaxTicket, uint32 *pcbTicket );
	// int (*GetGameBadgeLevel)( int nSeries, bool bFoil );
	// int (*GetPlayerSteamLevel)();
	// SteamAPICall_t (*RequestStoreAuthURL)( const char *pchRedirectURL );
	// bool (*BIsPhoneVerified)();
	// bool (*BIsTwoFactorEnabled)();
	// bool (*BIsPhoneIdentifying)();
	// bool (*BIsPhoneRequiringVerification)();
	// SteamAPICall_t (*GetMarketEligibility)();
	// SteamAPICall_t (*GetDurationControl)();
	// bool (*BSetDurationControlOnlineState)( EDurationControlOnlineState eNewState );
};

struct ISteamUser *SteamUser023(void);

}

#endif
