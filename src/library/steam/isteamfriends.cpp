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

#include "isteamfriends.h"

#include "logging.h"

#include <string>

namespace libtas {

namespace {

std::string currentPersonaName;

}

const char *ISteamFriends::GetPersonaName()
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    if (currentPersonaName.empty())
        return "";

    return currentPersonaName.c_str();
}

SteamAPICall_t ISteamFriends::SetPersonaName( const char *pchPersonaName )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    currentPersonaName = pchPersonaName ? pchPersonaName : "";
    return 0;
}

EPersonaState ISteamFriends::GetPersonaState()
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0; // k_EPersonaStateOffline
}

int ISteamFriends::GetFriendCount( int iFriendFlags )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

STEAMID_RET ISteamFriends::GetFriendByIndex( STEAMID_RET_ARG int iFriend, int iFriendFlags )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	STEAMID_RETURN(0);
}

EFriendRelationship ISteamFriends::GetFriendRelationship( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0; // k_EFriendRelationshipNone
}

EPersonaState ISteamFriends::GetFriendPersonaState( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0; // k_EPersonaStateOffline
}

const char *ISteamFriends::GetFriendPersonaName( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return "";
}

bool ISteamFriends::GetFriendGamePlayed( CSteamID steamIDFriend, FriendGameInfo_t *pFriendGameInfo )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

const char *ISteamFriends::GetFriendPersonaNameHistory( CSteamID steamIDFriend, int iPersonaName )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return "";
}

int ISteamFriends::GetFriendSteamLevel( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

const char *ISteamFriends::GetPlayerNickname( CSteamID steamIDPlayer )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
    return nullptr;
}

int ISteamFriends::GetFriendsGroupCount()
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

FriendsGroupID_t ISteamFriends::GetFriendsGroupIDByIndex( int iFG )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

const char *ISteamFriends::GetFriendsGroupName( FriendsGroupID_t friendsGroupID )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return "";
}

int ISteamFriends::GetFriendsGroupMembersCount( FriendsGroupID_t friendsGroupID )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

void ISteamFriends::GetFriendsGroupMembersList( FriendsGroupID_t friendsGroupID, CSteamID *pOutSteamIDMembers, int nMembersCount )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

bool ISteamFriends::HasFriend( CSteamID steamIDFriend, int iFriendFlags )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

int ISteamFriends::GetClanCount()
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

STEAMID_RET ISteamFriends::GetClanByIndex( STEAMID_RET_ARG int iClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	STEAMID_RETURN(0);
}

const char *ISteamFriends::GetClanName( CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return "";
}

const char *ISteamFriends::GetClanTag( CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return "";
}

bool ISteamFriends::GetClanActivityCounts( CSteamID steamIDClan, int *pnOnline, int *pnInGame, int *pnChatting )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

SteamAPICall_t ISteamFriends::DownloadClanActivityCounts( CSteamID *psteamIDClans, int cClansToRequest )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

int ISteamFriends::GetFriendCountFromSource( CSteamID steamIDSource )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

STEAMID_RET ISteamFriends::GetFriendFromSourceByIndex( STEAMID_RET_ARG CSteamID steamIDSource, int iFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	STEAMID_RETURN(0);
}

bool ISteamFriends::IsUserInSource( CSteamID steamIDUser, CSteamID steamIDSource )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

void ISteamFriends::SetInGameVoiceSpeaking( CSteamID steamIDUser, bool bSpeaking )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

void ISteamFriends::ActivateGameOverlay( const char *pchDialog )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

void ISteamFriends::ActivateGameOverlayToUser( const char *pchDialog, CSteamID steamID )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

void ISteamFriends::ActivateGameOverlayToWebPage( const char *pchURL )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

void ISteamFriends::ActivateGameOverlayToStore( AppId_t nAppID, EOverlayToStoreFlag eFlag )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

void ISteamFriends::SetPlayedWith( CSteamID steamIDUserPlayedWith )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

void ISteamFriends::ActivateGameOverlayInviteDialog( CSteamID steamIDLobby )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

int ISteamFriends::GetSmallFriendAvatar( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

int ISteamFriends::GetMediumFriendAvatar( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

int ISteamFriends::GetLargeFriendAvatar( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

bool ISteamFriends::RequestUserInformation( CSteamID steamIDUser, bool bRequireNameOnly )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

SteamAPICall_t ISteamFriends::RequestClanOfficerList( CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

STEAMID_RET ISteamFriends::GetClanOwner( STEAMID_RET_ARG CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	STEAMID_RETURN(0);
}

int ISteamFriends::GetClanOfficerCount( CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

STEAMID_RET ISteamFriends::GetClanOfficerByIndex( STEAMID_RET_ARG CSteamID steamIDClan, int iOfficer )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	STEAMID_RETURN(0);
}

uint32_t ISteamFriends::GetUserRestrictions()
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

bool ISteamFriends::SetRichPresence( const char *pchKey, const char *pchValue )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return true;
}

void ISteamFriends::ClearRichPresence()
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

const char *ISteamFriends::GetFriendRichPresence( CSteamID steamIDFriend, const char *pchKey )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return "";
}

int ISteamFriends::GetFriendRichPresenceKeyCount( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

const char *ISteamFriends::GetFriendRichPresenceKeyByIndex( CSteamID steamIDFriend, int iKey )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return "";
}

void ISteamFriends::RequestFriendRichPresence( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

bool ISteamFriends::InviteUserToGame( CSteamID steamIDFriend, const char *pchConnectString )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

int ISteamFriends::GetCoplayFriendCount()
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

STEAMID_RET ISteamFriends::GetCoplayFriend( STEAMID_RET_ARG int iCoplayFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	STEAMID_RETURN(0);
}

int ISteamFriends::GetFriendCoplayTime( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

AppId_t ISteamFriends::GetFriendCoplayGame( CSteamID steamIDFriend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

SteamAPICall_t ISteamFriends::JoinClanChatRoom( CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

bool ISteamFriends::LeaveClanChatRoom( CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

int ISteamFriends::GetClanChatMemberCount( CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

STEAMID_RET ISteamFriends::GetChatMemberByIndex( STEAMID_RET_ARG CSteamID steamIDClan, int iUser )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	STEAMID_RETURN(0);
}

bool ISteamFriends::SendClanChatMessage( CSteamID steamIDClanChat, const char *pchText )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

int ISteamFriends::GetClanChatMessage( CSteamID steamIDClanChat, int iMessage, void *prgchText, int cchTextMax, EChatEntryType *peChatEntryType, CSteamID *psteamidChatter )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

bool ISteamFriends::IsClanChatAdmin( CSteamID steamIDClanChat, CSteamID steamIDUser )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

bool ISteamFriends::IsClanChatWindowOpenInSteam( CSteamID steamIDClanChat )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

bool ISteamFriends::OpenClanChatWindowInSteam( CSteamID steamIDClanChat )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

bool ISteamFriends::CloseClanChatWindowInSteam( CSteamID steamIDClanChat )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

bool ISteamFriends::SetListenForFriendsMessages( bool bInterceptEnabled )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

bool ISteamFriends::ReplyToFriendMessage( CSteamID steamIDFriend, const char *pchMsgToSend )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

int ISteamFriends::GetFriendMessage( CSteamID steamIDFriend, int iMessageID, void *pvData, int cubData, EChatEntryType *peChatEntryType )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

SteamAPICall_t ISteamFriends::GetFollowerCount( CSteamID steamID )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

SteamAPICall_t ISteamFriends::IsFollowing( CSteamID steamID )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

SteamAPICall_t ISteamFriends::EnumerateFollowingList( uint32_t unStartIndex )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

bool ISteamFriends::IsClanPublic( CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

bool ISteamFriends::IsClanOfficialGameGroup( CSteamID steamIDClan )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

int ISteamFriends::GetNumChatsWithUnreadPriorityMessages()
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return 0;
}

void ISteamFriends::ActivateGameOverlayRemotePlayTogetherInviteDialog( CSteamID steamIDLobby )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

bool ISteamFriends::RegisterProtocolInTheOverlay( const char *pchProtocol )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
	return false;
}

void ISteamFriends::ActivateGameOverlayInviteDialogConnectString( const char *pchConnectString )
{
    LOGTRACE_SIMPLE(LCF_STEAM);
}

}
