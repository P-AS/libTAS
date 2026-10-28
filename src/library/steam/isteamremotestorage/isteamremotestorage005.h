//====== Copyright (c) 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#ifndef LIBTAS_ISTEAMREMOTESTORAGE005_H_INCL
#define LIBTAS_ISTEAMREMOTESTORAGE005_H_INCL

#include "isteamremotestorage.h"

#include <stdint.h>
#define STEAMREMOTESTORAGE_INTERFACE_VERSION_004 "STEAMREMOTESTORAGE_INTERFACE_VERSION004"
#define STEAMREMOTESTORAGE_INTERFACE_VERSION_005 "STEAMREMOTESTORAGE_INTERFACE_VERSION005"

namespace libtas {

struct ISteamRemoteStorage005Vtbl
{
    bool (STEAM_CALL *FileWrite)( void* iface, const char *pchFile, const void *pvData, int cubData );
    int (STEAM_CALL *FileRead)( void* iface, const char *pchFile, void *pvData, int cubDataToRead );
    bool (STEAM_CALL *FileForget)( void* iface, const char *pchFile );
    bool (STEAM_CALL *FileDelete)( void* iface, const char *pchFile );
    SteamAPICall_t (STEAM_CALL *FileShare)( void* iface, const char *pchFile );
    bool (STEAM_CALL *SetSyncPlatforms)( void* iface, const char *pchFile, ERemoteStoragePlatform eRemoteStoragePlatform );
    bool (STEAM_CALL *FileExists)( void* iface, const char *pchFile );
    bool (STEAM_CALL *FilePersisted)( void* iface, const char *pchFile );
    int (STEAM_CALL *GetFileSize)( void* iface, const char *pchFile );
    int64_t (STEAM_CALL *GetFileTimestamp)( void* iface, const char *pchFile );
    ERemoteStoragePlatform (STEAM_CALL *GetSyncPlatforms)( void* iface, const char *pchFile );
    int (STEAM_CALL *GetFileCount)(void* iface);
    const char *(STEAM_CALL *GetFileNameAndSize)( void* iface, int iFile, int *pnFileSizeInBytes );
    bool (STEAM_CALL *GetQuota)( void* iface, uint64_t *pnTotalBytes, uint64_t *puAvailableBytes );
    bool (STEAM_CALL *IsCloudEnabledForAccount)(void* iface);
    bool (STEAM_CALL *IsCloudEnabledForApp)(void* iface);
    void (STEAM_CALL *SetCloudEnabledForApp)( void* iface, bool bEnabled );
    SteamAPICall_t (STEAM_CALL *UGCDownload)( void* iface, UGCHandle_t hContent, unsigned int unPriority );
    bool (STEAM_CALL *GetUGCDetails)( void* iface, UGCHandle_t hContent, AppId_t *pnAppID, char **ppchName, int *pnFileSizeInBytes, CSteamID *pSteamIDOwner );
    int (STEAM_CALL *UGCRead)( void* iface, UGCHandle_t hContent, void *pvData, int cubDataToRead, unsigned int cOffset, EUGCReadAction eAction );
    int (STEAM_CALL *GetCachedUGCCount)(void* iface);
    UGCHandle_t (STEAM_CALL *GetCachedUGCHandle)( void* iface, int iCachedContent );
    SteamAPICall_t (STEAM_CALL *PublishFileOld)( void* iface, const char *pchFile, const char *pchPreviewFile, AppId_t nConsumerAppId, const char *pchTitle, const char *pchDescription, ERemoteStoragePublishedFileVisibility eVisibility, SteamParamStringArray_t *pTags );
    SteamAPICall_t (STEAM_CALL *PublishWorkshopFileOld)( void* iface, const char *pchFile, const char *pchPreviewFile, AppId_t nConsumerAppId, const char *pchTitle, const char *pchDescription, SteamParamStringArray_t *pTags );
    SteamAPICall_t (STEAM_CALL *CommitPublishedFileUpdate)( void* iface, PublishedFileUpdateHandle_t updateHandle );
    SteamAPICall_t (STEAM_CALL *GetPublishedFileDetails)( void* iface, PublishedFileId_t unPublishedFileId, unsigned int unMaxSecondsOld );
    SteamAPICall_t (STEAM_CALL *DeletePublishedFile)( void* iface, PublishedFileId_t unPublishedFileId );
    SteamAPICall_t (STEAM_CALL *EnumerateUserPublishedFiles)( void* iface, unsigned int unStartIndex );
    SteamAPICall_t (STEAM_CALL *SubscribePublishedFile)( void* iface, PublishedFileId_t unPublishedFileId );
    SteamAPICall_t (STEAM_CALL *EnumerateUserSubscribedFiles)( void* iface, unsigned int unStartIndex );
    SteamAPICall_t (STEAM_CALL *UnsubscribePublishedFile)( void* iface, PublishedFileId_t unPublishedFileId );
};

struct ISteamRemoteStorage *SteamRemoteStorage005(void);

}

#endif
