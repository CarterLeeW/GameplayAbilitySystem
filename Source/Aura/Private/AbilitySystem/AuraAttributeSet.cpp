// Copyright Carter Wooton


#include "AbilitySystem/AuraAttributeSet.h"
#include <Net/UnrealNetwork.h>
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

#define MAP_TAG_TO_ATTRIBUTE(Type, Attr) TagsToAttributes.Add(GameplayTags->Attributes_##Type##_##Attr, Get##Attr##Attribute());
UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags* GameplayTags = FAuraGameplayTags::Get();
	
	// Primary
	MAP_TAG_TO_ATTRIBUTE(Primary, Strength);
	MAP_TAG_TO_ATTRIBUTE(Primary, Intelligence);
	MAP_TAG_TO_ATTRIBUTE(Primary, Resilience);
	MAP_TAG_TO_ATTRIBUTE(Primary, Vigor);

	// Secondary
	MAP_TAG_TO_ATTRIBUTE(Secondary, Armor);
	MAP_TAG_TO_ATTRIBUTE(Secondary, ArmorPenetration);
	MAP_TAG_TO_ATTRIBUTE(Secondary, BlockChance);
	MAP_TAG_TO_ATTRIBUTE(Secondary, CriticalHitChance);
	MAP_TAG_TO_ATTRIBUTE(Secondary, CriticalHitDamage);
	MAP_TAG_TO_ATTRIBUTE(Secondary, CriticalHitResistance);
	MAP_TAG_TO_ATTRIBUTE(Secondary, HealthRegen);
	MAP_TAG_TO_ATTRIBUTE(Secondary, ManaRegen);
	MAP_TAG_TO_ATTRIBUTE(Secondary, MaxHealth);
	MAP_TAG_TO_ATTRIBUTE(Secondary, MaxMana);

	// Resistance
	TagsToAttributes.Add(GameplayTags->Attributes_Resistance_Arcane, GetArcaneResistanceAttribute());
	TagsToAttributes.Add(GameplayTags->Attributes_Resistance_Fire, GetFireResistanceAttribute());
	TagsToAttributes.Add(GameplayTags->Attributes_Resistance_Lightning, GetLightningResistanceAttribute());
	TagsToAttributes.Add(GameplayTags->Attributes_Resistance_Physical, GetPhysicalResistanceAttribute());
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Resistance
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);

	// Vital
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

#pragma region "OnRep"

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HeathRegeneration(const FGameplayAttributeData& OldHeathRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegen, OldHeathRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegen, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

#pragma endregion "OnRep"

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);

	if (Properties.TargetCharacter->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_IsDead(Properties.TargetCharacter))
		{
			return;
		}
	}

	// Handle IncomingDamage Meta Attribute, all of this will be on server
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Properties);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingExpAttribute())
	{
		HandleIncomingExp(Properties);
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}



/** Used internally in PostGameplayEffectExecute */
void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source == causer of the effect, Target == target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// Source
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.Get())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		// If AbilityActorInfo does not have the source controller, then get it directly from the source actor
		if (!Props.SourceController && Props.SourceAvatarActor)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	// Target
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Properties)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (!bFatal) // not fatal - Play HitReaction ability
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FAuraGameplayTags::Get()->Effects_HitReact);
			Properties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}
		else if (bFatal) // is fatal
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Properties.TargetAvatarActor))
			{
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Properties.EffectContextHandle));
			}
			SendExpEvent(Properties);
		}
		// Show damage widget
		ShowDamageWidget(Properties, LocalIncomingDamage);
		if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Properties.EffectContextHandle))
		{
			HandleDebuffs(Properties);
		}
	}
}

void UAuraAttributeSet::HandleDebuffs(const FEffectProperties& Properties)
{
	const FAuraGameplayTags* Tags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Properties.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Properties.SourceAvatarActor);

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Properties.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Properties.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Properties.EffectContextHandle);
	const float DebuffPeriod = UAuraAbilitySystemLibrary::GetDebuffPeriod(Properties.EffectContextHandle);
	FName DebuffName = FName(FString::Printf(TEXT("DynamicDebuff %s"), *DamageType.ToString()));

	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), DebuffName);

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffPeriod;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	Effect->bExecutePeriodicEffectOnApplication = false;
	//Effect->InheritableOwnedTagsContainer.AddTag(Tags->DamageTypesToDebuffs[DamageType]); // Deprecated!!!
	/* This is the correct way for 5.3 using UTargetTagsGameplayEffectComponent */
	UTargetTagsGameplayEffectComponent& TargetTags = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer GrantedTags;
	GrantedTags.Added.AddTag(Tags->DamageTypesToDebuffs[DamageType]);
	//GrantedTags.Added.AddTag()
	TargetTags.SetAndApplyTargetTagChanges(GrantedTags);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	// First modifier
	int32 Index = Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UAuraAttributeSet::GetIncomingDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContext.Get()))
		{
			TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
			AuraContext->SetDamageType(DebuffDamageType);
			Properties.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
		}
	}
}

void UAuraAttributeSet::ShowDamageWidget(const FEffectProperties& Properties, const float LocalIncomingDamage)
{
	if (Properties.SourceCharacter != Properties.TargetCharacter)
	{
		if (AAuraPlayerController* SPC = Cast<AAuraPlayerController>(Properties.SourceCharacter->Controller))
		{
			SPC->ShowDamageNumber(
				Properties.TargetCharacter,
				LocalIncomingDamage,
				UAuraAbilitySystemLibrary::IsBlockedHit(Properties.EffectContextHandle),
				UAuraAbilitySystemLibrary::IsCriticalHit(Properties.EffectContextHandle)
			);
		}
		else if (AAuraPlayerController* TPC = Cast<AAuraPlayerController>(Properties.TargetCharacter->Controller))
		{
			TPC->ShowDamageNumber(
				Properties.TargetCharacter,
				LocalIncomingDamage,
				UAuraAbilitySystemLibrary::IsBlockedHit(Properties.EffectContextHandle),
				UAuraAbilitySystemLibrary::IsCriticalHit(Properties.EffectContextHandle)
			);
		}
	}
}

void UAuraAttributeSet::HandleIncomingExp(const FEffectProperties& Properties)
{
	const float LocalIncomingExp = GetIncomingExp();
	SetIncomingExp(0.f);

	// Source character is the owner
	if (Properties.SourceCharacter->Implements<UPlayerInterface>() && Properties.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Properties.SourceCharacter);
		const int32 CurrentExp = IPlayerInterface::Execute_GetExp(Properties.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForExp(Properties.SourceCharacter, CurrentExp + LocalIncomingExp);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			// TODO: maybe make this recursive to handle multiple level ups?
			const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(Properties.SourceCharacter, CurrentLevel);
			const int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(Properties.SourceCharacter, CurrentLevel);

			IPlayerInterface::Execute_AddToPlayerLevel(Properties.SourceCharacter, NumLevelUps);
			IPlayerInterface::Execute_AddToAttributePoints(Properties.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Properties.SourceCharacter, SpellPointsReward);

			bTopOffHealth = true;
			bTopOffMana = true;

			IPlayerInterface::Execute_LevelUp(Properties.SourceCharacter);
		}

		IPlayerInterface::Execute_AddToExp(Properties.SourceCharacter, LocalIncomingExp);
	}
}

void UAuraAttributeSet::SendExpEvent(const FEffectProperties& Properties)
{
	if (Properties.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Properties.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Properties.TargetCharacter);
		const int32 ExpReward = UAuraAbilitySystemLibrary::GetExpRewardForCharacterClassAndLevel(Properties.TargetCharacter, TargetClass, TargetLevel);

		FGameplayEventData Payload;
		Payload.EventTag = FAuraGameplayTags::Get()->Attributes_Meta_IncomingExp;
		Payload.EventMagnitude = static_cast<float>(ExpReward);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Properties.SourceCharacter, FAuraGameplayTags::Get()->Attributes_Meta_IncomingExp, Payload);
	}
}
