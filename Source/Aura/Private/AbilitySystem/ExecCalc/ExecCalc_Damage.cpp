// Copyright Carter Wooton


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AuraAbilityTypes.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
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
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCI = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCI = Cast<ICombatInterface>(TargetAvatar);
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

	// 1.
	// Get Damage set by Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get()->Damage);

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
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCI->GetPlayerLevel(), 1.f);
	// Calculate Effective Armor using TargetArmor and SourceArmorPenetration
	const float EffectiveArmor = FMath::Max<float>(TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f, 0.f);
	const FRealCurve* EffectiveArmorCurve = CCI->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCI->GetPlayerLevel(), 1.f);

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
		const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCI->GetPlayerLevel(), 1.f);

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
