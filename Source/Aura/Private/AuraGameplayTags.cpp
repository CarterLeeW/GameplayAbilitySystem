// Copyright Carter Wooton


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "NativeGameplayTags.h"

#define ADD_TAG(tag, name, comment) GameplayTags->tag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(name), FString(comment))
#define MAP_DAM_TO_RES(type) GameplayTags->DamageTypesToResistances.Add(GameplayTags->Damage_##type, GameplayTags->Attributes_Resistance_##type);
#define ATTRIBUTES_PRIMARY "Attributes.Primary"
#define ATTRIBUTES_SECONDARY "Attributes.Secondary"
#define ATTRIBUTES_RESISTANCE "Attributes.Resistance"
#define DAMAGE "Damage"
#define INPUTTAG "InputTag"

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

	ADD_TAG(Attributes_Resistance_Arcane,    ATTRIBUTES_RESISTANCE".Arcane",    "Resistance to Arcane Damage");
	ADD_TAG(Attributes_Resistance_Fire,      ATTRIBUTES_RESISTANCE".Fire",      "Resistance to Fire Damage");
	ADD_TAG(Attributes_Resistance_Lightning, ATTRIBUTES_RESISTANCE".Lightning", "Resistance to Lightning Damage");
	ADD_TAG(Attributes_Resistance_Physical,  ATTRIBUTES_RESISTANCE".Physical",  "Resistance to Physical Damage");

	ADD_TAG(Damage,           DAMAGE,             "Generic Damage Type");
	ADD_TAG(Damage_Arcane,    DAMAGE".Arcane",    "Arcane Damage Type");
	ADD_TAG(Damage_Fire,      DAMAGE".Fire",      "Fire Damage Type");
	ADD_TAG(Damage_Lightning, DAMAGE".Lightning", "Lightning Damage Type");
	ADD_TAG(Damage_Physical,  DAMAGE".Physical",  "Physical Damage Type");
	// Map of damage types to resistances, must be updated each time a resistance or damage is added
	MAP_DAM_TO_RES(Arcane);
	MAP_DAM_TO_RES(Fire);
	MAP_DAM_TO_RES(Lightning);
	MAP_DAM_TO_RES(Physical);

	ADD_TAG(InputTag_LMB, INPUTTAG".LMB", "Input Tag for Left Mouse Button");
	ADD_TAG(InputTag_RMB, INPUTTAG".RMB", "Input Tag for Right Mouse Button");
	ADD_TAG(InputTag_1,   INPUTTAG".1",   "Input Tag for 1 Key Button");
	ADD_TAG(InputTag_2,   INPUTTAG".2",   "Input Tag for 2 Key Button");
	ADD_TAG(InputTag_3,   INPUTTAG".3",   "Input Tag for 3 Key Button");
	ADD_TAG(InputTag_4,   INPUTTAG".4",   "Input Tag for 4 Key Button");

	ADD_TAG(Effects_HitReact, "Effects.HitReact", "Handles what happens when a pawn is hit with an object or spell");

	ADD_TAG(Ability_Attack, "Ability.Attack", "Tag for attacking gameplay ability");
	return GameplayTags;
}
