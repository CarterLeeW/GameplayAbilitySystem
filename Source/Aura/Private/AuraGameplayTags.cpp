// Copyright Carter Wooton


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "NativeGameplayTags.h"

FAuraGameplayTags* FAuraGameplayTags::GameplayTags = nullptr;

FAuraGameplayTags* FAuraGameplayTags::Get()
{
	check(GameplayTags);
	return GameplayTags;
}

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
#define ADD_TAG(attribute, name, comment) GameplayTags->attribute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(name), FString(comment));


	if (GameplayTags)
	{
		return;
	}

	GameplayTags = new FAuraGameplayTags();

#pragma region "Secondary Attributes"
#define ATTRIBUTES_SECONDARY "Attributes.Secondary"

	ADD_TAG(Attributes_Secondary_Armor,                   ATTRIBUTES_SECONDARY".Armor",                  "Reduces damage taken, improves Block Chance");
	ADD_TAG(Attributes_Secondary_ArmorPenetration,        ATTRIBUTES_SECONDARY".ArmorPenetration",       "comment");
	ADD_TAG(Attributes_Secondary_BlockChance,             ATTRIBUTES_SECONDARY".BlockChance",            "comment");
	ADD_TAG(Attributes_Secondary_CriticalHitChance,       ATTRIBUTES_SECONDARY".CriticalHitChance",      "comment");
	ADD_TAG(Attributes_Secondary_CriticalHitDamage,       ATTRIBUTES_SECONDARY".CriticalHitDamage",      "comment");
	ADD_TAG(Attributes_Secondary_CriticalHitResistance,   ATTRIBUTES_SECONDARY".CriticalHitResistance",  "comment");
	ADD_TAG(Attributes_Secondary_HealthRegen,             ATTRIBUTES_SECONDARY".HealthRegen",            "comment");
	ADD_TAG(Attributes_Secondary_ManaRegen,               ATTRIBUTES_SECONDARY".ManaRegen",              "comment");
	ADD_TAG(Attributes_Secondary_MaxHealth,               ATTRIBUTES_SECONDARY".MaxHealth",              "comment");
	ADD_TAG(Attributes_Secondary_MaxMana,                 ATTRIBUTES_SECONDARY".MaxMana",                "comment");
#pragma endregion "Secondary Attributes"

}