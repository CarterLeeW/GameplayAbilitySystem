// Copyright Carter Wooton


#include "AbilitySystem/AsyncTask/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = InAbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(InAbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// To know when a cooldown has ended (Cooldown Tag has been removed)
	InAbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(WaitCooldownChange, &UWaitCooldownChange::CooldownTagChanged);

	// To know when a cooldown effect has been applied
	WaitCooldownChange->ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

		SetReadyToDestroy();
		MarkAsGarbage();
	}
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		const TArray<float> RemainingTimes = ASC->GetActiveEffectsTimeRemaining(Query);
		const float TimeRemaining = FMath::Max<float>(RemainingTimes);

		CooldownStart.Broadcast(TimeRemaining);
	}
}
