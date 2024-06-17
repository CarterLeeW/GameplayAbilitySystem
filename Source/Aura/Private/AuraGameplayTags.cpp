// Copyright Carter Wooton


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "NativeGameplayTags.h"
#include "AuraGameplayTagMacros.h"

#define ADD_TAG(tag, name, comment) GameplayTags->tag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(name), FString(comment))

FAuraGameplayTags* FAuraGameplayTags::GameplayTags = nullptr;

FAuraGameplayTags* FAuraGameplayTags::Get()
{
	if (GameplayTags)
	{
		return GameplayTags;
	}

	GameplayTags = new FAuraGameplayTags();

	ADD_TAG(Attributes_Primary_Strength,     TAG_ATTRIBUTES_PRIMARY".Strength",     "Increases physical damage");
	ADD_TAG(Attributes_Primary_Intelligence, TAG_ATTRIBUTES_PRIMARY".Intelligence", "Increases magical damage");
	ADD_TAG(Attributes_Primary_Resilience,   TAG_ATTRIBUTES_PRIMARY".Resilience",   "Increases Armor and Armor Penetration");
	ADD_TAG(Attributes_Primary_Vigor,        TAG_ATTRIBUTES_PRIMARY".Vigor",        "Increases Health");

	ADD_TAG(Attributes_Secondary_Armor,                 TAG_ATTRIBUTES_SECONDARY".Armor",                 "Reduces damage taken, improves Block Chance");
	ADD_TAG(Attributes_Secondary_ArmorPenetration,      TAG_ATTRIBUTES_SECONDARY".ArmorPenetration",      "Ignores percentage of enemy Armor, increases Critical Hit Chance");
	ADD_TAG(Attributes_Secondary_BlockChance,           TAG_ATTRIBUTES_SECONDARY".BlockChance",           "Chance to cut incoming damage in half");
	ADD_TAG(Attributes_Secondary_CriticalHitChance,     TAG_ATTRIBUTES_SECONDARY".CriticalHitChance",     "Chance to double damage plus critical hit bonus");
	ADD_TAG(Attributes_Secondary_CriticalHitDamage,     TAG_ATTRIBUTES_SECONDARY".CriticalHitDamage",     "Bonus damage added when a critical hit is scored");
	ADD_TAG(Attributes_Secondary_CriticalHitResistance, TAG_ATTRIBUTES_SECONDARY".CriticalHitResistance", "Reduces Critical Hit Chance of attacking enemies");
	ADD_TAG(Attributes_Secondary_HealthRegen,           TAG_ATTRIBUTES_SECONDARY".HealthRegen",           "Amount of Health regenerated every 1 second");
	ADD_TAG(Attributes_Secondary_ManaRegen,             TAG_ATTRIBUTES_SECONDARY".ManaRegen",             "Amount of Mana regenerated every 1 second");
	ADD_TAG(Attributes_Secondary_MaxHealth,             TAG_ATTRIBUTES_SECONDARY".MaxHealth",             "Maximum amount of Health obtainable");
	ADD_TAG(Attributes_Secondary_MaxMana,               TAG_ATTRIBUTES_SECONDARY".MaxMana",               "Maximum amount of Mana obtainable");

	ADD_TAG(Attributes_Resistance_Arcane,    TAG_ATTRIBUTES_RESISTANCE".Arcane",    "Resistance to Arcane Damage");
	ADD_TAG(Attributes_Resistance_Fire,      TAG_ATTRIBUTES_RESISTANCE".Fire",      "Resistance to Fire Damage");
	ADD_TAG(Attributes_Resistance_Lightning, TAG_ATTRIBUTES_RESISTANCE".Lightning", "Resistance to Lightning Damage");
	ADD_TAG(Attributes_Resistance_Physical,  TAG_ATTRIBUTES_RESISTANCE".Physical",  "Resistance to Physical Damage");

	ADD_TAG(Attributes_Meta_IncomingExp, TAG_ATTRIBUTES_META".IncomingExp", "Tag for IncomingExp meta attribute");

	ADD_TAG(Damage,           TAG_DAMAGE,             "Generic Damage Type");
	ADD_TAG(Damage_Arcane,    TAG_DAMAGE".Arcane",    "Arcane Damage Type");
	ADD_TAG(Damage_Fire,      TAG_DAMAGE".Fire",      "Fire Damage Type");
	ADD_TAG(Damage_Lightning, TAG_DAMAGE".Lightning", "Lightning Damage Type");
	ADD_TAG(Damage_Physical,  TAG_DAMAGE".Physical",  "Physical Damage Type");

	ADD_TAG(Debuff_Arcane,   TAG_DEBUFF".Arcane",   "Arcane damage debuff");
	ADD_TAG(Debuff_Burn,     TAG_DEBUFF".Burn",     "Burn damage debuff");
	ADD_TAG(Debuff_Physical, TAG_DEBUFF".Physical", "Physical damage debuff");
	ADD_TAG(Debuff_Stun,     TAG_DEBUFF".Stun",     "Stun damage debuff");


	// Map of damage types to resistances, must be updated each time a resistance or damage is added
	GameplayTags->DamageTypesToResistances.Add(GameplayTags->Damage_Arcane, GameplayTags->Attributes_Resistance_Arcane);
	GameplayTags->DamageTypesToResistances.Add(GameplayTags->Damage_Fire, GameplayTags->Attributes_Resistance_Fire);
	GameplayTags->DamageTypesToResistances.Add(GameplayTags->Damage_Lightning, GameplayTags->Attributes_Resistance_Lightning);
	GameplayTags->DamageTypesToResistances.Add(GameplayTags->Damage_Physical, GameplayTags->Attributes_Resistance_Physical);

	// Map of damage types to debuffs, must be updated each time a debuff or damage is added
	GameplayTags->DamageTypesToDebuffs.Add(GameplayTags->Damage_Arcane, GameplayTags->Debuff_Arcane);
	GameplayTags->DamageTypesToDebuffs.Add(GameplayTags->Damage_Fire, GameplayTags->Debuff_Burn);
	GameplayTags->DamageTypesToDebuffs.Add(GameplayTags->Damage_Physical, GameplayTags->Debuff_Physical);
	GameplayTags->DamageTypesToDebuffs.Add(GameplayTags->Damage_Lightning, GameplayTags->Debuff_Stun);

	ADD_TAG(InputTag_LMB,       TAG_INPUTTAG".LMB",       "Input Tag for Left Mouse Button");
	ADD_TAG(InputTag_RMB,       TAG_INPUTTAG".RMB",       "Input Tag for Right Mouse Button");
	ADD_TAG(InputTag_1,         TAG_INPUTTAG".1",         "Input Tag for 1 Key Button");
	ADD_TAG(InputTag_2,         TAG_INPUTTAG".2",         "Input Tag for 2 Key Button");
	ADD_TAG(InputTag_3,         TAG_INPUTTAG".3",         "Input Tag for 3 Key Button");
	ADD_TAG(InputTag_4,         TAG_INPUTTAG".4",         "Input Tag for 4 Key Button");
	ADD_TAG(InputTag_Passive_1, TAG_INPUTTAG_PASSIVE".1", "Input Tag for Passive 1");
	ADD_TAG(InputTag_Passive_2, TAG_INPUTTAG_PASSIVE".2", "Input Tag for Passive 2");

	ADD_TAG(Effects_HitReact, "Effects.HitReact", "Handles what happens when a pawn is hit with an object or spell");

	ADD_TAG(Ability_Attack,        TAG_ABILITY".Attack",   "Tag for attacking gameplay ability");
	ADD_TAG(Ability_Summon,        TAG_ABILITY".Summon",   "Tag for summon gameplay ability");
	ADD_TAG(Ability_HitReact,      TAG_ABILITY".HitReact", "Tag for HitReact ability");
	ADD_TAG(Ability_None,          TAG_ABILITY".None",     "Null ability tag");

	ADD_TAG(Ability_Fire_FireBolt, TAG_ABILITY_FIRE".FireBolt", "Tag for firebolt gameplay ability");

	ADD_TAG(Ability_Lightning_Electrocute, TAG_ABILITY_LIGHTNING".Electrocute", "Electrocute gameplay ability");

	ADD_TAG(Ability_Status_Locked,   TAG_ABILITY_STATUS".Locked", "Tag for locked ability");
	ADD_TAG(Ability_Status_Eligible, TAG_ABILITY_STATUS".Eligible", "Tag for Eligible ability");
	ADD_TAG(Ability_Status_Unlocked, TAG_ABILITY_STATUS".Unlocked", "Tag for Unlocked ability");
	ADD_TAG(Ability_Status_Equipped, TAG_ABILITY_STATUS".Equipped", "Tag for Equipped ability");

	ADD_TAG(Ability_Type_Offensive, TAG_ABILITY_TYPE".Offensive", "Tag for offensive ability");
	ADD_TAG(Ability_Type_Passive,   TAG_ABILITY_TYPE".Passive",   "Tag for passive ability");
	ADD_TAG(Ability_Type_None,      TAG_ABILITY_TYPE".None",      "Tag for ability with no type category required");

	ADD_TAG(Cooldown_Fire_Firebolt, TAG_COOLDOWN_FIRE".FireBolt", "Tag for firebolt GA cooldown");

	ADD_TAG(CombatSocket_Weapon,    TAG_COMBATSOCKET".Weapon",    "Weapon CombatSocket");
	ADD_TAG(CombatSocket_RightHand, TAG_COMBATSOCKET".RightHand", "Right hand CombatSocket");
	ADD_TAG(CombatSocket_LeftHand,  TAG_COMBATSOCKET".LeftHand",  "Left hand CombatSocket");
	ADD_TAG(CombatSocket_Tail,      TAG_COMBATSOCKET".Tail",      "Tail CombatSocket");

	ADD_TAG(Montage_Attack_1, TAG_MONTAGE_ATTACK".1", "First montage attack tag");
	ADD_TAG(Montage_Attack_2, TAG_MONTAGE_ATTACK".2", "Second montage attack tag");
	ADD_TAG(Montage_Attack_3, TAG_MONTAGE_ATTACK".3", "Third montage attack tag");
	ADD_TAG(Montage_Attack_4, TAG_MONTAGE_ATTACK".4", "Fourth montage attack tag");

	return GameplayTags;
}
