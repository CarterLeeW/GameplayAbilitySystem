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

	// Primary Attributes
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	// Secondary Attributes
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegen;
	FGameplayTag Attributes_Secondary_ManaRegen;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	// Resistances
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Physical;

	// Meta
	FGameplayTag Attributes_Meta_IncomingExp;

	// Damage Types
	FGameplayTag Damage;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Physical;

	// Debuff Types
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Physical;
	FGameplayTag Debuff_Stun;

	// Debuff Stats
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Period;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	// Input Tags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	FGameplayTag Effects_HitReact;

	// Ability Tags
	FGameplayTag Ability_Attack;
	FGameplayTag Ability_Summon;
	FGameplayTag Ability_HitReact;
	FGameplayTag Ability_None;

	FGameplayTag Ability_Fire_FireBolt;
	FGameplayTag Ability_Lightning_Electrocute;

	FGameplayTag Ability_Status_Locked;
	FGameplayTag Ability_Status_Eligible;
	FGameplayTag Ability_Status_Unlocked;
	FGameplayTag Ability_Status_Equipped;

	FGameplayTag Ability_Type_Offensive;
	FGameplayTag Ability_Type_Passive;
	FGameplayTag Ability_Type_None;

	// Cooldown Tags
	FGameplayTag Cooldown_Fire_Firebolt;

	// Socker Tags
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	// Montage Attack Tags
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	FAuraGameplayTags(const FAuraGameplayTags& obj) = delete;

private:
	FAuraGameplayTags() {};
	static FAuraGameplayTags* GameplayTags;
};
