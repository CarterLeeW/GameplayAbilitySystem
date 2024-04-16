// Copyright Carter Wooton


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "NativeGameplayTags.h"

#define ADD_TAG(tag, name, comment) GameplayTags->tag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(name), FString(comment));
#define ATTRIBUTES_PRIMARY "Attributes.Primary"
#define ATTRIBUTES_SECONDARY "Attributes.Secondary"
#define INPUTTAG "InputTag"
#define DAMAGE "Damage"

FAuraGameplayTags* FAuraGameplayTags::GameplayTags = nullptr;

FAuraGameplayTags* FAuraGameplayTags::Get()
{
	if (GameplayTags)
	{
		return GameplayTags;
	}

	GameplayTags = new FAuraGameplayTags();

	ADD_TAG(Attributes_Primary_Strength,     ATTRIBUTES_PRIMARY".Strength",     "Increases physical damage");
	ADD_TAG(Attributes_Primary_Intelligence, ATTRIBUTES_PRIMARY".Intelligence", "Increases magical damage");
	ADD_TAG(Attributes_Primary_Resilience,   ATTRIBUTES_PRIMARY".Resilience",   "Increases Armor and Armor Penetration");
	ADD_TAG(Attributes_Primary_Vigor,        ATTRIBUTES_PRIMARY".Vigor",        "Increases Health");

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

	ADD_TAG(InputTag_LMB, INPUTTAG".LMB", "Input Tag for Left Mouse Button");
	ADD_TAG(InputTag_RMB, INPUTTAG".RMB", "Input Tag for Right Mouse Button");
	ADD_TAG(InputTag_1,   INPUTTAG".1",   "Input Tag for 1 Key Button");
	ADD_TAG(InputTag_2,   INPUTTAG".2",   "Input Tag for 2 Key Button");
	ADD_TAG(InputTag_3,   INPUTTAG".3",   "Input Tag for 3 Key Button");
	ADD_TAG(InputTag_4,   INPUTTAG".4",   "Input Tag for 4 Key Button");

	ADD_TAG(Damage, DAMAGE, "Generic Damage Type");
	ADD_TAG(Damage_Fire, DAMAGE".Fire", "Fire Damage Type");
	// Add all damage types to array
	GameplayTags->DamageTypes.Add(GameplayTags->Damage_Fire);

	ADD_TAG(Effects_HitReact, "Effects.HitReact", "Handles what happens when a pawn is hit with an object or spell");

	return GameplayTags;
}
