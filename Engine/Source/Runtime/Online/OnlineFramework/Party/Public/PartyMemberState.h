// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PartyMemberState.generated.h"

class UPartyGameState;

/**
 * Simple struct for replication and copying of party member data on updates
 */
USTRUCT()
struct FPartyMemberRepState
{
	GENERATED_USTRUCT_BODY()

	/** Reset the variables of this party member state */
	virtual void Reset()
	{}
};

/**
 * Main representation of a party member
 */
UCLASS(config = Game, notplaceable)
class PARTY_API UPartyMemberState : public UObject
{
	GENERATED_UCLASS_BODY()

	// Begin UObject Interface
	virtual void BeginDestroy() override;
	// End UObject Interface

	/** Unique id of this party member */
	UPROPERTY(Transient)
	FUniqueNetIdRepl UniqueId;

	/** Display name of this party member */
	UPROPERTY(BlueprintReadOnly, Transient, Category = PartyMemberState)
	FText DisplayName;

	/** @return the party this member is associated with */
	UPartyGameState* GetParty() const;

protected:

	/** Reflection data for child USTRUCT */
	UPROPERTY()
	const UScriptStruct* MemberStateRefDef;

	/** Pointer to child USTRUCT that holds the current state of party member (set via InitPartyMemberState) */
	FPartyMemberRepState* MemberStateRef;

	template<typename T>
	void InitPartyMemberState(T* InMemberState)
	{
		MemberStateRef = InMemberState;
		if (MemberStateRefDef == nullptr)
		{
			MemberStateRefDef = T::StaticStruct();
		}

		MemberStateRefScratch = (FPartyMemberRepState*)FMemory::Malloc(MemberStateRefDef->GetCppStructOps()->GetSize());
		MemberStateRefDef->InitializeStruct(MemberStateRefScratch);
	}

	void UpdatePartyMemberState();

	/**
	 * Compare current data to old data, triggering delegates
	 *
	 * @param OldPartyMemberState old view of data to compare against
	 */
	virtual void ComparePartyMemberData(const FPartyMemberRepState& OldPartyMemberState);

	/** Reset to default state */
	virtual void Reset();

private:
	

	/** Scratch copy of child USTRUCT for handling replication comparisons */
	FPartyMemberRepState* MemberStateRefScratch;

	/** Have we announced this player joining the game locally */
	UPROPERTY(Transient)
	bool bHasAnnouncedJoin;

	friend UPartyGameState;
};
