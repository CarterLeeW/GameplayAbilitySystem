// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * Singleton containing native Gameplay Tags
 */

struct FAuraGameplayTags
{
public:
	static FAuraGameplayTags* Get();
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;
	FAuraGameplayTags(const FAuraGameplayTags& obj) = delete;
protected:

private:
	FAuraGameplayTags() {};
	static FAuraGameplayTags* GameplayTags;
};
