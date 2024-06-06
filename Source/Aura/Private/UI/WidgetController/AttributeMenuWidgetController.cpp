// Copyright Carter Wooton


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>(PlayerState);
	check(AuraPS);
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}
	OnPlayerAttributePointsChanged.Broadcast(AuraPS->GetAttributePoints());
	OnPlayerSpellPointsChanged.Broadcast(AuraPS->GetSpellPoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>(PlayerState);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value);
			}
		);
	}

	// Player Stats
	AuraPS->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 NewAttributePoints) { OnPlayerAttributePointsChanged.Broadcast(NewAttributePoints); }
	);
	AuraPS->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 NewSpellPoints) { OnPlayerSpellPointsChanged.Broadcast(NewSpellPoints); }
	);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag, true);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
