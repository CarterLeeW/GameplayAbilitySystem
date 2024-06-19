// Copyright Carter Wooton


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AuraAbilityTypes.h"

#define DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(set, prop, targetsource, bcapture) \
{ \
	prop##Property = FindFieldChecked<FProperty>(set::StaticClass(), ((void)sizeof(UEAsserts_Private::GetMemberNameCheckedJunk(((set*)0)->Get##prop())), FName(TEXT(#prop)))); \
	prop##Def = FGameplayEffectAttributeCaptureDefinition(prop##Property, EGameplayEffectAttributeCaptureSource::targetsource, bcapture); \
}

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	// Resistances
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, CriticalHitResistance, Target, false);

		// Resistances
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF_LOCAL(UAuraAttributeSet, PhysicalResistance, Target, false)

		// Map tags to capture defs
		const FAuraGameplayTags* Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags->Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags->Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags->Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags->Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags->Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

		TagsToCaptureDefs.Add(Tags->Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags->Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags->Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags->Attributes_Resistance_Physical, PhysicalResistanceDef);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	// Resistances
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const FAuraGameplayTags* Tags = FAuraGameplayTags::Get();


	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}

	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const UCharacterClassInfo* CCI = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	/* Begin calculation 
	* Order:
	* 1. Get damage set by caller magnitude from Gameplay Ability
	* 2. Determine if a block was made, and halve damage if true
	* 3. Use Source ArmorPenetration to reduce the Target's EffectiveArmor
	* 4. Apply EffectiveArmor modifier to Damage
	* 5. Determine if hit was Critical using Source's CriticalHitChance and Target's CriticalHitResistance
	* 6. Perform calculations on Damage using Source's CriticalHitDamage
	*/

	// Debuff
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : Tags->DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (TypeDamage >= 0.f)
		{
			// Determine if there was a successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(Tags->Debuff_Chance, false, -1.f);
			const bool bDebuff = FMath::RandRange(0.f, 100.f) < SourceDebuffChance;
			if (bDebuff)
			{
				// TODO: Apply debuff
			}
		}
	}

	// 1.
	// Get Damage types set by Caller Magnitude and check resistances
	float Damage = 0.f;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : Tags->DamageTypesToResistances)
	{
		const FGameplayTag& DamageTypeTag = Pair.Key;
		const FGameplayTag& ResistanceTag = Pair.Value;

		const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false, 0.f);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;
		Damage += DamageTypeValue;
	}

	// Capture BlockChance on Target, and determine if there was a successful block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	// 2.
	// If Block, halve the Damage
	const bool bShouldBlock = TargetBlockChance > FMath::FRandRange(0.f, 100.f) ? true : false;
	if (bShouldBlock)
	{
		Damage /= 2.f;
		UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, true);
	}

	// Capture Armor on Target
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	// 3.
	// Capture ArmorPenetration on Source, ignores a percentage of the Target's Armor
	// *These calculations are subject to game design choices
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const FRealCurve* ArmorPenetrationCurve = CCI->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel, 1.f);
	// Calculate Effective Armor using TargetArmor and SourceArmorPenetration
	const float EffectiveArmor = FMath::Max<float>(TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f, 0.f);
	const FRealCurve* EffectiveArmorCurve = CCI->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel, 1.f);

	// 4.
	// Reduce Damage value by effective armor
	Damage *= (100.f - (EffectiveArmor * EffectiveArmorCoefficient)) / 100.f;

	// 5.
	// Determine if hit was Critical using Source's CriticalHitChance and Target's CriticalHitResistance
	if (!bShouldBlock)
	{
		float SourceCriticalHitChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
		SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

		float TargetCriticalHitResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
		TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

		const FRealCurve* CriticalHitResistanceCurve = CCI->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
		const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel, 1.f);

		const bool bShouldCrit = SourceCriticalHitChance > FMath::FRandRange(0.f, 100.f) ? true : false;
		const bool bShouldResist = TargetCriticalHitResistance > FMath::FRandRange(0.f, 100.f) ? true : false;

		if (bShouldCrit && !bShouldResist)
		{
			UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, true);
			float SourceCriticalHitDamage = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
			SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);


			// 6.
			// Perform Calculations on Damage
			Damage = Damage * ((2 + (SourceCriticalHitDamage / 100.f)));
		}
	}




	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
