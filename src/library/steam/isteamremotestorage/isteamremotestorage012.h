//====== Copyright (c) 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to user account information in Steam
//
//=============================================================================

#ifndef LIBTAS_ISTEAMREMOTESTORAGE012_H_INCL
#define LIBTAS_ISTEAMREMOTESTORAGE012_H_INCL

#include "isteamremotestorage.h"

#include <stdint.h>
#define STEAMREMOTESTORAGE_INTERFACE_VERSION_008 "STEAMREMOTESTORAGE_INTERFACE_VERSION008"
#define STEAMREMOTESTORAGE_INTERFACE_VERSION_009 "STEAMREMOTESTORAGE_INTERFACE_VERSION009"
#define STEAMREMOTESTORAGE_INTERFACE_VERSION_010 "STEAMREMOTESTORAGE_INTERFACE_VERSION010"
#define STEAMREMOTESTORAGE_INTERFACE_VERSION_011 "STEAMREMOTESTORAGE_INTERFACE_VERSION011"
#define STEAMREMOTESTORAGE_INTERFACE_VERSION_012 "STEAMREMOTESTORAGE_INTERFACE_VERSION012"

namespace libtas {

struct ISteamRemoteStorage012Vtbl
{
    bool (STEAM_CALL *FileWrite)( void* iface, const char *pchFile, const void *pvData, int cubData );
    int (STEAM_CALL *FileRead)( void* iface, const char *pchFile, void *pvData, int cubDataToRead );
    bool (STEAM_CALL *FileForget)( void* iface, const char *pchFile );
    bool (STEAM_CALL *FileDelete)( void* iface, const char *pchFile );
    SteamAPICall_t (STEAM_CALL *FileShare)( void* iface, const char *pchFile );
    bool (STEAM_CALL *SetSyncPlatforms)( void* iface, const char *pchFile, ERemoteStoragePlatform eRemoteStoragePlatform );
    UGCFileWriteStreamHandle_t (STEAM_CALL *FileWriteStreamOpen)( void* iface, const char *pchFile );
    bool (STEAM_CALL *FileWriteStreamWriteChunk)( void* iface, UGCFileWriteStreamHandle_t writeHandle, const void *pvData, int cubData );
    bool (STEAM_CALL *FileWriteStreamClose)( void* iface, UGCFileWriteStreamHandle_t writeHandle );
    bool (STEAM_CALL *FileWriteStreamCancel)( void* iface, UGCFileWriteStreamHandle_t writeHandle );
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
    bool (STEAM_CALL *GetUGCDownloadProgress)( void* iface, UGCHandle_t hContent, int *pnBytesDownloaded, int *pnBytesExpected );
    bool (STEAM_CALL *GetUGCDetails)( void* iface, UGCHandle_t hContent, AppId_t *pnAppID, char **ppchName, int *pnFileSizeInBytes, CSteamID *pSteamIDOwner );
    int (STEAM_CALL *UGCRead)( void* iface, UGCHandle_t hContent, void *pvData, int cubDataToRead, unsigned int cOffset, EUGCReadAction eAction );
    int (STEAM_CALL *GetCachedUGCCount)(void* iface);
    UGCHandle_t (STEAM_CALL *GetCachedUGCHandle)( void* iface, int iCachedContent );
    SteamAPICall_t (STEAM_CALL *PublishWorkshopFile)( void* iface, const char *pchFile, const char *pchPreviewFile, AppId_t nConsumerAppId, const char *pchTitle, const char *pchDescription, ERemoteStoragePublishedFileVisibility eVisibility, SteamParamStringArray_t *pTags, EWorkshopFileType eWorkshopFileType );
    PublishedFileUpdateHandle_t (STEAM_CALL *CreatePublishedFileUpdateRequest)( void* iface, PublishedFileId_t unPublishedFileId );
    bool (STEAM_CALL *UpdatePublishedFileFile)( void* iface, PublishedFileUpdateHandle_t updateHandle, const char *pchFile );
    bool (STEAM_CALL *UpdatePublishedFilePreviewFile)( void* iface, PublishedFileUpdateHandle_t updateHandle, const char *pchPreviewFile );
    bool (STEAM_CALL *UpdatePublishedFileTitle)( void* iface, PublishedFileUpdateHandle_t updateHandle, const char *pchTitle );
    bool (STEAM_CALL *UpdatePublishedFileDescription)( void* iface, PublishedFileUpdateHandle_t updateHandle, const char *pchDescription );
    bool (STEAM_CALL *UpdatePublishedFileVisibility)( void* iface, PublishedFileUpdateHandle_t updateHandle, ERemoteStoragePublishedFileVisibility eVisibility );
    bool (STEAM_CALL *UpdatePublishedFileTags)( void* iface, PublishedFileUpdateHandle_t updateHandle, SteamParamStringArray_t *pTags );
    SteamAPICall_t (STEAM_CALL *CommitPublishedFileUpdate)( void* iface, PublishedFileUpdateHandle_t updateHandle );
    SteamAPICall_t (STEAM_CALL *GetPublishedFileDetails)( void* iface, PublishedFileId_t unPublishedFileId, unsigned int unMaxSecondsOld );
    SteamAPICall_t (STEAM_CALL *DeletePublishedFile)( void* iface, PublishedFileId_t unPublishedFileId );
    SteamAPICall_t (STEAM_CALL *EnumerateUserPublishedFiles)( void* iface, unsigned int unStartIndex );
    SteamAPICall_t (STEAM_CALL *SubscribePublishedFile)( void* iface, PublishedFileId_t unPublishedFileId );
    SteamAPICall_t (STEAM_CALL *EnumerateUserSubscribedFiles)( void* iface, unsigned int unStartIndex );
    SteamAPICall_t (STEAM_CALL *UnsubscribePublishedFile)( void* iface, PublishedFileId_t unPublishedFileId );
    bool (STEAM_CALL *UpdatePublishedFileSetChangeDescription)( void* iface, PublishedFileUpdateHandle_t updateHandle, const char *pchChangeDescription );
    SteamAPICall_t (STEAM_CALL *GetPublishedItemVoteDetails)( void* iface, PublishedFileId_t unPublishedFileId );
    SteamAPICall_t (STEAM_CALL *UpdateUserPublishedItemVote)( void* iface, PublishedFileId_t unPublishedFileId, bool bVoteUp );
    SteamAPICall_t (STEAM_CALL *GetUserPublishedItemVoteDetails)( void* iface, PublishedFileId_t unPublishedFileId );
    SteamAPICall_t (STEAM_CALL *EnumerateUserSharedWorkshopFiles)( void* iface, CSteamID steamId, unsigned int unStartIndex, SteamParamStringArray_t *pRequiredTags, SteamParamStringArray_t *pExcludedTags );
    SteamAPICall_t (STEAM_CALL *PublishVideo)( void* iface, EWorkshopVideoProvider eVideoProvider, const char *pchVideoAccount, const char *pchVideoIdentifier, const char *pchPreviewFile, AppId_t nConsumerAppId, const char *pchTitle, const char *pchDescription, ERemoteStoragePublishedFileVisibility eVisibility, SteamParamStringArray_t *pTags );
    SteamAPICall_t (STEAM_CALL *SetUserPublishedFileAction)( void* iface, PublishedFileId_t unPublishedFileId, EWorkshopFileAction eAction );
    SteamAPICall_t (STEAM_CALL *EnumeratePublishedFilesByUserAction)( void* iface, EWorkshopFileAction eAction, unsigned int unStartIndex );
    SteamAPICall_t (STEAM_CALL *EnumeratePublishedWorkshopFiles)( void* iface, EWorkshopEnumerationType eEnumerationType, unsigned int unStartIndex, unsigned int unCount, unsigned int unDays, SteamParamStringArray_t *pTags, SteamParamStringArray_t *pUserTags );
    SteamAPICall_t (STEAM_CALL *UGCDownloadToLocation)( void* iface, UGCHandle_t hContent, const char *pchLocation, unsigned int unPriority );
};

struct ISteamRemoteStorage *SteamRemoteStorage012(void);

}

#endif
