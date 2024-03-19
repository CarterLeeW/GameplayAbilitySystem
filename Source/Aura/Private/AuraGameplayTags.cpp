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

#pragma region "Primary Attributes"
#define ATTRIBUTES_PRIMARY "Attributes.Primary"

	ADD_TAG(Attributes_Primary_Strength,     ATTRIBUTES_PRIMARY".Strength",     "Increases physical damage");
	ADD_TAG(Attributes_Primary_Intelligence, ATTRIBUTES_PRIMARY".Intelligence", "Increases magical damage");
	ADD_TAG(Attributes_Primary_Resilience,   ATTRIBUTES_PRIMARY".Resilience",   "Increases Armor and Armor Penetration");
	ADD_TAG(Attributes_Primary_Vigor,        ATTRIBUTES_PRIMARY".Vigor",        "Increases Health");

#pragma endregion "Primary Attributes"

#pragma region "Secondary Attributes"
#define ATTRIBUTES_SECONDARY "Attributes.Secondary"

	ADD_TAG(Attributes_Secondary_Armor,                 ATTRIBUTES_SECONDARY".Armor",                 "Reduces damage taken, improves Block Chance");
	ADD_TAG(Attributes_Secondary_ArmorPenetration,      ATTRIBUTES_SECONDARY".ArmorPenetration",      "Ignores percentage of enemy Armor, increases Critical Hit Chance");
	ADD_TAG(Attributes_Secondary_BlockChance,           ATTRIBUTES_SECONDARY".BlockChance",           "Chance to cut incoming damage in half");
	ADD_TAG(Attributes_Secondary_CriticalHitChance,     ATTRIBUTES_SECONDARY".CriticalHitChance",     "Chance to double damage plus critical hit bonus");
	ADD_TAG(Attributes_Secondary_CriticalHitDamage,     ATTRIBUTES_SECONDARY".CriticalHitDamage",     "Bonus damage added when a critical hit is scored");
	ADD_TAG(Attributes_Secondary_CriticalHitResistance, ATTRIBUTES_SECONDARY".CriticalHitResistance", "Reduces Critical Hit Chance of attacking enemies");
	ADD_TAG(Attributes_Secondary_HealthRegen,           ATTRIBUTES_SECONDARY".HealthRegen",           "Amount of Health regenerated every 1 second");
	ADD_TAG(Attributes_Secondary_ManaRegen,             ATTRIBUTES_SECONDARY".ManaRegen",             "Amount of Mana regenerated every 1 second");
	ADD_TAG(Attributes_Secondary_MaxHealth,             ATTRIBUTES_SECONDARY".MaxHealth",             "Maximum amount of Health obtainable");
	ADD_TAG(Attributes_Secondary_MaxMana,               ATTRIBUTES_SECONDARY".MaxMana",               "Maximum amount of Mana obtainable");
#pragma endregion "Secondary Attributes"

}