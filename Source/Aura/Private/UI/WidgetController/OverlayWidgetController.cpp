// Copyright Carter Wooton


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"


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
