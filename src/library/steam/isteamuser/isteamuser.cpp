/*
    Copyright 2015-2026 Clément Gallet <clement.gallet@ens-lyon.org>

    This file is part of libTAS.

    libTAS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libTAS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libTAS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "isteamuser.h"
#include "isteamuser_priv.h"
#include "isteamuser021.h"
#include "isteamuser023.h"

#include "steam/CCallback.h"
#include "steam/CCallbackManager.h"

#include "logging.h"
#include "hook.h"
#include "Utils.h"
#include "global.h"

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h> 

namespace libtas {

static char steamuserdir[2048] = "/NOTVALID";
static const char *steamuser_version = NULL;

void SteamSetUserDataFolder(std::string path)
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    strncpy(steamuserdir, path.c_str(), sizeof(steamuserdir)-1);
    steamuserdir[sizeof(steamuserdir)-1] = '\0';
}

struct ISteamUser *SteamUser_generic(const char *version)
{
    static const struct
    {
        const char *name;
        struct ISteamUser *(*iface_getter)(void);
    } ifaces[] = {
        { STEAMUSER_INTERFACE_VERSION_016, SteamUser021 },
        { STEAMUSER_INTERFACE_VERSION_017, SteamUser021 },
        { STEAMUSER_INTERFACE_VERSION_018, SteamUser021 },
        { STEAMUSER_INTERFACE_VERSION_019, SteamUser021 },
        { STEAMUSER_INTERFACE_VERSION_020, SteamUser021 },
        { STEAMUSER_INTERFACE_VERSION_021, SteamUser021 },
        { STEAMUSER_INTERFACE_VERSION_023, SteamUser023 },
        { NULL, NULL }
    };
    int i;

    LOG(LL_DEBUG, LCF_STEAM, "%s called with version %s", __func__, version);

    i = 0;
    while (ifaces[i].name)
    {
        if (strcmp(ifaces[i].name, version) == 0)
        {
            if (ifaces[i].iface_getter)
                return ifaces[i].iface_getter();

            break;
        }
        i++;
    }

    LOG(LL_WARN, LCF_STEAM, "Unable to find ISteamUser version %s", version);

    return nullptr;
}

void SteamUser_set_version(const char *version)
{
    LOG(LL_DEBUG, LCF_STEAM, "%s called with version %s", __func__, version);

    if (!steamuser_version)
        steamuser_version = version;
}

struct ISteamUser *SteamUser(void)
{
    LOGTRACE_SIMPLE(LCF_STEAM);

    if (!Global::shared_config.virtual_steam) {
        RETURN_NATIVE(SteamUser, (), "steam_api");
    }

    static struct ISteamUser *cached_iface = nullptr;

    if (!steamuser_version)
    {
        steamuser_version = STEAMUSER_INTERFACE_VERSION_023;
        LOG(LL_WARN, LCF_STEAM, "ISteamUser: No version specified, defaulting to %s", steamuser_version);
    }

    if (!cached_iface)
        cached_iface = SteamUser_generic(steamuser_version);

    return cached_iface;
}

struct ISteamUser *SteamAPI_SteamUser_v023(void)
{
    LOGTRACE_SIMPLE(LCF_STEAM);

    if (!Global::shared_config.virtual_steam) {
        RETURN_NATIVE(SteamAPI_SteamUser_v023, (), "steam_api");
    }

    static struct ISteamUser *cached_iface = nullptr;

    steamuser_version = STEAMUSER_INTERFACE_VERSION_023;
    if (!cached_iface)
        cached_iface = SteamUser_generic(steamuser_version);

    return cached_iface;
}

HSteamUser STEAM_CALL ISteamUser_GetHSteamUser( void* iface )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    return 1;
}

bool STEAM_CALL ISteamUser_BLoggedOn( void* iface )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    return false; // Required for N++ to pass the splash screen
}

STEAMID_RET STEAM_CALL ISteamUser_GetSteamID( void* iface STEAMID_RET_ARG_LAST )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    STEAMID_RETURN(1);
}

int STEAM_CALL ISteamUser_InitiateGameConnection( void* iface, void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, unsigned int unIPServer, uint16_t usPortServer, bool bSecure )
{
    LOGTRACE_SIMPLE(LCF_STEAM);

    if(!pAuthBlob || cbMaxAuthBlob < 1)
        return 0;

    *(char *)pAuthBlob = 0;
    return 1;
}

void STEAM_CALL ISteamUser_TerminateGameConnection( void* iface, unsigned int unIPServer, uint16_t usPortServer )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

void STEAM_CALL ISteamUser_TrackAppUsageEvent( void* iface, CGameID gameID, int eAppUsageEvent, const char *pchExtraInfo )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

bool STEAM_CALL ISteamUser_GetUserDataFolder( void* iface, char *pchBuffer, int cubBuffer )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    if (pchBuffer == nullptr || cubBuffer <= 0)
        return false;

    strncpy(pchBuffer, steamuserdir, cubBuffer-1);
    pchBuffer[cubBuffer-1] = '\0';
    LOG(LL_DEBUG, LCF_STEAM, "user data folder = \"%s\".", steamuserdir);
    return true;
}

void STEAM_CALL ISteamUser_StartVoiceRecording( void* iface )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

void STEAM_CALL ISteamUser_StopVoiceRecording( void* iface )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

EVoiceResult STEAM_CALL ISteamUser_GetAvailableVoice( void* iface, unsigned int *pcbCompressed, unsigned int *pcbUncompressed_Deprecated, unsigned int nUncompressedVoiceDesiredSampleRate_Deprecated)
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    return 3; //k_EVoiceResultNoData
}

EVoiceResult STEAM_CALL ISteamUser_GetVoice( void* iface, bool bWantCompressed, void *pDestBuffer, unsigned int cbDestBufferSize, unsigned int *nBytesWritten, bool bWantUncompressed_Deprecated, void *pUncompressedDestBuffer_Deprecated, unsigned int cbUncompressedDestBufferSize_Deprecated, unsigned int *nUncompressBytesWritten_Deprecated, unsigned int nUncompressedVoiceDesiredSampleRate_Deprecated)
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    return 3; //k_EVoiceResultNoData
}

EVoiceResult STEAM_CALL ISteamUser_DecompressVoice( void* iface, const void *pCompressed, unsigned int cbCompressed, void *pDestBuffer, unsigned int cbDestBufferSize, unsigned int *nBytesWritten, unsigned int nDesiredSampleRate )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    return 3; //k_EVoiceResultNoData
}

unsigned int STEAM_CALL ISteamUser_GetVoiceOptimalSampleRate( void* iface )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    return 44100;
}

HAuthTicket STEAM_CALL ISteamUser_GetAuthSessionTicket( void* iface, void *pTicket, int cbMaxTicket, unsigned int *pcbTicket )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    // if (pcbTicket)
    //     *pcbTicket = 8;
    return 1;
}

HAuthTicket STEAM_CALL ISteamUser_GetAuthTicketForWebApi( void* iface, const char *pchIdentity )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    // if (pcbTicket)
    //     *pcbTicket = 8;
    return 1;
}
    
EBeginAuthSessionResult STEAM_CALL ISteamUser_BeginAuthSession( void* iface, const void *pAuthTicket, int cbAuthTicket, CSteamID steamID )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    return 0; //k_EBeginAuthSessionResultOK;
}

void STEAM_CALL ISteamUser_EndAuthSession( void* iface, CSteamID steamID )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

void STEAM_CALL ISteamUser_CancelAuthTicket( void* iface, HAuthTicket hAuthTicket )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

}
