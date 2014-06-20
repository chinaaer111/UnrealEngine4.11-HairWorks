
#include "AbilitySystemPrivatePCH.h"
#include "Abilities/Tasks/AbilityTask_WaitAbilityActivate.h"

#include "AbilitySystemComponent.h"

#include "Abilities/GameplayAbility.h"

UAbilityTask_WaitAbilityActivate::UAbilityTask_WaitAbilityActivate(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
}

UAbilityTask_WaitAbilityActivate* UAbilityTask_WaitAbilityActivate::WaitForAbilityActivate(UObject* WorldContextObject, FGameplayTag InWithTag, FGameplayTag InWithoutTag)
{
	check(WorldContextObject);
	UGameplayAbility* ThisAbility = CastChecked<UGameplayAbility>(WorldContextObject);
	if (ThisAbility)
	{
		UAbilityTask_WaitAbilityActivate * MyObj = NULL;
		MyObj = NewObject<UAbilityTask_WaitAbilityActivate>();
		MyObj->Ability = ThisAbility;
		MyObj->WithTag = InWithTag;
		MyObj->WithoutTag = InWithoutTag;

		return MyObj;
	}
	return NULL;
}

void UAbilityTask_WaitAbilityActivate::Activate()
{
	if (Ability.IsValid())
	{
		const FGameplayAbilityActorInfo* Info = Ability.Get()->GetCurrentActorInfo();

		UAbilitySystemComponent* ASC = Info->AbilitySystemComponent.Get();
		ASC->AbilityActivatedCallbacks.AddUObject(this, &UAbilityTask_WaitAbilityActivate::OnAbilityActivate);
	}
}

void UAbilityTask_WaitAbilityActivate::OnAbilityActivate(UGameplayAbility *ActivatedAbility)
{
	if ((WithTag.IsValid() && !ActivatedAbility->AbilityTags.HasTag(WithTag, EGameplayTagMatchType::IncludeParentTags, EGameplayTagMatchType::Explicit)) ||
		(WithoutTag.IsValid() && ActivatedAbility->AbilityTags.HasTag(WithoutTag, EGameplayTagMatchType::IncludeParentTags, EGameplayTagMatchType::Explicit)))
	{
		// Failed tag check
		return;
	}

	UAbilitySystemComponent* ASC = Ability.Get()->GetCurrentActorInfo()->AbilitySystemComponent.Get();
	if (ASC)
	{
		ASC->AbilityActivatedCallbacks.RemoveUObject(this, &UAbilityTask_WaitAbilityActivate::OnAbilityActivate);
	}

	OnActivate.Broadcast(ActivatedAbility);
}