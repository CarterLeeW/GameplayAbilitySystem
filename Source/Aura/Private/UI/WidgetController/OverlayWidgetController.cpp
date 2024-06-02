// Copyright Carter Wooton


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
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
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// Health
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	// Mana
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>(PlayerState);
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	BIND_ATTRIBUTE_CALLBACK(Health);
	BIND_ATTRIBUTE_CALLBACK(MaxHealth);
	BIND_ATTRIBUTE_CALLBACK(Mana);
	BIND_ATTRIBUTE_CALLBACK(MaxMana);

	/* If abilities are set, then we can call this function directly.
	If abilities are not set, then we must bind this function to the delegate */
	if (AuraASC->bStartupAbilitiesGiven)
	{
		OnInitializeStartupAbilities(AuraASC);
	}
	else
	{
		AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
	}

	// Capture tags from EffectApplied in Ability System Component
	AuraASC->EffectAssetTagsDelegate.AddLambda(
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
	AuraPS->OnExpChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnExpChanged);
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
{
	// TODO Get information about all given abilities, look up their ability info, and broadcast it to widgets
	if (!AuraASC->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = UAuraAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	AuraASC->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnExpChanged(int32 NewExp)
{
	PlayerExpDelegate.Broadcast(NewExp);
}
