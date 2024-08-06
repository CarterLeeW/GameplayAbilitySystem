// Copyright Carter Wooton


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.0f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, Exp);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/* This overwrites the current level with the value provided. Use AddToLevel to increment value */
void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = FMath::Clamp(InLevel, 1, InLevel);
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	Level = FMath::Clamp(Level, 1, Level);
	OnLevelChangedDelegate.Broadcast(Level, true);
}

/* This overwrites the current Exp with the value provided. Use AddToExp to increment value */
void AAuraPlayerState::SetExp(int32 InExp)
{
	Exp = FMath::Clamp(InExp, 0, InExp);
	OnExpChangedDelegate.Broadcast(Exp);
}

void AAuraPlayerState::AddToExp(int32 InExp)
{
	Exp += InExp;
	Exp = FMath::Clamp(Exp, 0, Exp);
	OnExpChangedDelegate.Broadcast(Exp);
}

/* This overwrites the current AttributePoints with the value provided. Use AddToAttributePoints to increment value */
void AAuraPlayerState::SetAttributePoints(int32 InAttributePoints)
{
	AttributePoints = FMath::Clamp(InAttributePoints, 1, InAttributePoints);
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	AttributePoints = FMath::Clamp(AttributePoints, 0, AttributePoints);
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

/* This overwrites the current SpellPoints with the value provided. Use AddToSpellPoints to increment value */
void AAuraPlayerState::SetSpellPoints(int32 InSpellPoints)
{
	SpellPoints = FMath::Clamp(InSpellPoints, 1, InSpellPoints);
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	SpellPoints = FMath::Clamp(SpellPoints, 0, SpellPoints);
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AAuraPlayerState::OnRep_Exp(int32 OldExp)
{
	OnExpChangedDelegate.Broadcast(Exp);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
