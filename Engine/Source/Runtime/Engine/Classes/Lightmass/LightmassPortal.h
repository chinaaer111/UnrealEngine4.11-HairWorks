// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "LightmassPortal.generated.h"

UCLASS(hidecategories=(Collision, Attachment, Actor), MinimalAPI)
class ALightmassPortal : public AActor
{
	GENERATED_UCLASS_BODY()

private_subobject:
	UPROPERTY(Category = Portal, VisibleAnywhere, BlueprintReadOnly)
	class ULightmassPortalComponent* PortalComponent;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBillboardComponent* SpriteComponent;
#endif // WITH_EDITORONLY_DATA

public:	

	virtual bool IsLevelBoundsRelevant() const override { return false; }

	class ULightmassPortalComponent* GetPortalComponent() const { return PortalComponent; }
#if WITH_EDITORONLY_DATA
	/** Returns SpriteComponent subobject **/
	UBillboardComponent* GetSpriteComponent() const { return SpriteComponent; }
#endif

#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif // WITH_EDITOR
};



