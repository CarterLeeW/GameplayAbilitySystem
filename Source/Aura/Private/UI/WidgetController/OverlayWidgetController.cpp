// Copyright Carter Wooton


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"


#define BIND_ATTRIBUTE_CALLBACK( AttributeName ) \
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( \
		AuraAttributeSet->Get##AttributeName##Attribute()).AddLambda( \
			[this](const FOnAttributeChangeData& Data) \
			{\
				On##AttributeName##Changed.Broadcast(Data.NewValue); \
			});

void UOverlayWidgetController::BroadcastInitialValues()
{
	// Health
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	// Mana
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	BIND_ATTRIBUTE_CALLBACK(Health);
	BIND_ATTRIBUTE_CALLBACK(MaxHealth);
	BIND_ATTRIBUTE_CALLBACK(Mana);
	BIND_ATTRIBUTE_CALLBACK(MaxMana);

	/* If abilities are set, then we can call this function directly.
	If abilities are not set, then we must bind this function to the delegate */
	if (GetAuraASC()->bStartupAbilitiesGiven)
	{
		BroadcastAbilityInfo();
	}
	else
	{
		GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UAuraWidgetController::BroadcastAbilityInfo);
	}

	GetAuraASC()->OnAbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);

	// Capture tags from EffectApplied in Ability System Component
	GetAuraASC()->EffectAssetTagsDelegate.AddLambda(
		[this](const FGameplayTagContainer& AssetTags) 
		{
			for (const auto& Tag : AssetTags)
			{
				// "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
				const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					if (const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
					{
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		}
	);

	// PlayerState
	GetAuraPS()->OnExpChangedDelegate.AddLambda(
		[this](int32 NewExp) { OnPlayerExpChanged.Broadcast(NewExp); }
	);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel) { OnPlayerLevelChanged.Broadcast(NewLevel); }
	);
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	const FAuraGameplayTags* Tags = FAuraGameplayTags::Get();

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
}
