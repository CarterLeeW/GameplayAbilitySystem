// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraGameplayAbility.h"

FString UAuraGameplayAbility::GetDescription(int32 Level) const
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), 
		L"Default Ability Name -  LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level) const
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"),
		L"NextLevel - Causes much more damage", Level+1);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell locked until next level: %d</>"), Level);
}
