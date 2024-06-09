// Copyright Carter Wooton


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{	
	for (auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}
	OnPlayerAttributePointsChanged.Broadcast(GetAuraPS()->GetAttributePoints());
	OnPlayerSpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	for (auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value);
			}
		);
	}

	// Player Stats
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 NewAttributePoints) { OnPlayerAttributePointsChanged.Broadcast(NewAttributePoints); }
	);
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 NewSpellPoints) { OnPlayerSpellPointsChanged.Broadcast(NewSpellPoints); }
	);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag, true);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
