// Copyright Carter Wooton


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags* FAuraGameplayTags::GameplayTags = nullptr;

FAuraGameplayTags* FAuraGameplayTags::Get()
{
	check(GameplayTags);
	return GameplayTags;
}

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	if (!GameplayTags)
	{
		GameplayTags = new FAuraGameplayTags();

		GameplayTags->Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Block Change"));
	}
}