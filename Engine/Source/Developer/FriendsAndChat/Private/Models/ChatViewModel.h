// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

class FChatItemViewModel;
namespace EChatMessageType
{
	enum Type : uint8;
}

struct FSelectedFriend
{
	TSharedPtr<FUniqueNetId> UserID;
	FText FriendName;
};

class FChatViewModel
	: public TSharedFromThis<FChatViewModel>
	, public IChatViewModel
{
public:
	virtual TArray< TSharedRef<FChatItemViewModel > >& GetFilteredChatList() = 0;
	virtual FReply HandleSelectionChanged(TSharedRef<FChatItemViewModel> ItemSelected) = 0;
	virtual FText GetViewGroupText() const = 0;
	virtual FText GetChatGroupText() const = 0;
	virtual void EnumerateChatChannelOptionsList(TArray<EChatMessageType::Type>& OUTChannelType) = 0;
	virtual void SetChatChannel(const EChatMessageType::Type NewOption) = 0;
	virtual void SetWhisperChannel(const TSharedPtr<FSelectedFriend> InFriend) = 0;
	virtual void SetViewChannel(const EChatMessageType::Type NewOption) = 0;
	virtual void SendMessage(const FText NewMessage) = 0;
	virtual void SetTimeDisplayTransparency(const float TimeTransparency) = 0;
	virtual const float GetTimeTransparency() const = 0;
	virtual EChatMessageType::Type GetChatChannelType() const = 0;
	virtual const TArray<TSharedPtr<FSelectedFriend> >& GetRecentOptions() const = 0;
	virtual void SetChatFriend(TSharedPtr<IFriendItem> ChatFriend) = 0;
	virtual EVisibility GetSScrollBarVisibility() const = 0;
	virtual void SetAllowGlobalChat(bool bAllow) = 0;
	virtual bool IsGlobalChatEnabled() const = 0;
	virtual void SetCaptureFocus(bool bCaptureFocus) = 0;
	virtual const bool ShouldCaptureFocus() const = 0;

	DECLARE_EVENT(FChatViewModel, FChatListSetFocus)
	virtual FChatListSetFocus& OnChatListSetFocus() = 0;

	virtual ~FChatViewModel() {}
};

/**
 * Creates the implementation for an ChatViewModel.
 *
 * @return the newly created ChatViewModel implementation.
 */
FACTORY(TSharedRef< FChatViewModel >, FChatViewModel,
	const TSharedRef<class FFriendsMessageManager>& MessageManager);