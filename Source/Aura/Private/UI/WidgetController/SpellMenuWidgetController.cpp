// Copyright Carter Wooton


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraGameplayTags.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	OnPlayerSpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->OnAbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	GetAuraASC()->OnAbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 NewSpellPoints) { OnPlayerSpellPointsChanged.Broadcast(NewSpellPoints); }
	);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel) { OnPlayerLevelChanged.Broadcast(NewLevel); }
	);
}

void USpellMenuWidgetController::SpendSpellPointButtonPressed(const FGameplayTag& AbilityTag)
{
	GetAuraASC()->Server_SpendSpellPoint(AbilityTag);
}

void USpellMenuWidgetController::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	if (!GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, OutDescription, OutNextLevelDescription))
	{
		OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
		OutNextLevelDescription = FString();
	}
}

void USpellMenuWidgetController::EquipButtonPressed(const FGameplayTag& AbilityTag)
{
	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(AbilityTag);
	// Is selected, therefore has InputTag
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get()->Ability_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetInputTagFromAbilityTag(AbilityTag);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& AbilityTag, const FGameplayTag& Slot, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;
	// Do nothing if selecting passive slot with offensive ability and vice versa
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	// Ability selected, equip pressed, and spell row globe pressed. Time to assign abilities
	GetAuraASC()->Server_EquipAbility(AbilityTag, Slot);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	const FAuraGameplayTags* Tags = FAuraGameplayTags::Get();

	bWaitingForEquipSelection = false;
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = Tags->Ability_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlot;
	LastSlotInfo.AbilityTag = Tags->Ability_None;
	// Broadcast empty info if PrevSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	// Fill in new ability information
	AbilityInfoDelegate.Broadcast(Info);
	// Stop equipping animations in BP
}
