// Copyright Carter Wooton


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameModeBase.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		OutAuraHUD = Cast<AAuraHUD>(PC->GetHUD());
		if (OutAuraHUD)
		{
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PC->GetPlayerState<AAuraPlayerState>();
			OutWCParams.AbilitySystemComponent = PC->GetPlayerState<AAuraPlayerState>()->GetAbilitySystemComponent();
			OutWCParams.AttributeSet = PC->GetPlayerState<AAuraPlayerState>()->GetAttributeSet();
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	AAuraHUD* AuraHUD = nullptr;
	FWidgetControllerParams WCParams;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	AAuraHUD* AuraHUD = nullptr;
	FWidgetControllerParams WCParams;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	AAuraHUD* AuraHUD = nullptr;
	FWidgetControllerParams WCParams;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const float Level, UAbilitySystemComponent* ASC)
{
	const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGM) return;

	AActor* AvatarActor = ASC->GetAvatarActor();

	const UCharacterClassInfo* CharacterClassInfo = AuraGM->CharacterClassInfo;
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	

	// Primary Attributes | Class specific
	FGameplayEffectContextHandle PrimaryAttributesCH = ASC->MakeEffectContext();
	PrimaryAttributesCH.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSH = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesCH);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSH.Data.Get());

	// Secondary Attributes | Shared
	FGameplayEffectContextHandle SecondaryAttributesCH = ASC->MakeEffectContext();
	SecondaryAttributesCH.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSH = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesCH);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSH.Data.Get());

	// Vital Attributes | Shared
	FGameplayEffectContextHandle VitalAttributesCH = ASC->MakeEffectContext();
	VitalAttributesCH.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSH = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesCH);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSH.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, const ECharacterClass CharacterClass)
{
	const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGM) return;
	
	const UCharacterClassInfo* CharacterClassInfo = AuraGM->CharacterClassInfo;
	if (!CharacterClassInfo) return;
	// Common Abilities
	for (const auto& AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	// Class Abilities
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (const auto& AbilityClass : DefaultInfo.GameplayAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGM) return nullptr;

	return AuraGM->CharacterClassInfo;
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin, TArray<AActor*>& OutOverlappingActors)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (const bool bImplementsCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>())
			{
				if (const bool bIsAlive = !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
				{
					OutOverlappingActors.AddUnique(Overlap.GetActor());
				}
			}
		}
	}
}

TArray<AActor*> UAuraAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, TArray<AActor*> ActorsCopy, const FVector& Origin)
{
	if (ActorsCopy.Num() <= MaxTargets)
	{
		return ActorsCopy;
	}

	TArray<AActor*> ClosestTargets;

	ActorsCopy.Sort([Origin](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Distance(A.GetActorLocation(), Origin);
			float DistanceB = FVector::Distance(B.GetActorLocation(), Origin);
			return DistanceA < DistanceB;
		});

	const int32 Size = FMath::Min(MaxTargets, ActorsCopy.Num());
	for (int32 i = 0; i < Size; i++)
	{
		ClosestTargets.Add(ActorsCopy[i]);
	}
	return ClosestTargets;
}

bool UAuraAbilitySystemLibrary::IsNotFriend(const AActor* FirstActor, const AActor* SecondActor)
{
	const bool bFirstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool bSecondIsPlayer = SecondActor->ActorHasTag(FName("Player"));
	const bool bFirstIsEnemy = FirstActor->ActorHasTag(FName("Enemy"));
	const bool bSecondIsEnemy = SecondActor->ActorHasTag(FName("Enemy"));

	const bool bAreFriends = (bFirstIsPlayer && bSecondIsPlayer) || (bFirstIsEnemy && bSecondIsEnemy);

	return !bAreFriends;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGM) return nullptr;

	return AuraGM->AbilityInfo;
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
	const FAuraGameplayTags* Tags = FAuraGameplayTags::Get();

	FGameplayEffectContextHandle EffectContextHandle = Params.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Params.SourceAbilitySystemComponent->GetAvatarActor());
	SetDeathImpulse(EffectContextHandle, Params.DeathImpulse);
	const FGameplayEffectSpecHandle SpecHandle = Params.SourceAbilitySystemComponent->MakeOutgoingSpec(
		Params.DamageGameplayEffectClass,
		Params.AbilityLevel, 
		EffectContextHandle
	);
	// Assign magnitudes
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Params.DamageType, Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags->Debuff_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags->Debuff_Damage, Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags->Debuff_Duration, Params.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags->Debuff_Period, Params.DebuffPeriod);

	Params.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

int32 UAuraAbilitySystemLibrary::GetExpRewardForCharacterClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGM) return int32();

	const UCharacterClassInfo* CharacterClassInfo = AuraGM->CharacterClassInfo;
	if (!CharacterClassInfo) return int32();

	const FCharacterClassDefaultInfo& CCInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float ExpReward = CCInfo.ExpReward.GetValueAtLevel(CharacterLevel);


	return static_cast<int32>(ExpReward);
}
