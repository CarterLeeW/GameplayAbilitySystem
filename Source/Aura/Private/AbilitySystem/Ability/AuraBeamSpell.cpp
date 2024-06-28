// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraBeamSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraBeamSpell::CacheMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraBeamSpell::TraceFirstTarget()
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HR;
			const FVector SocketLocation = Weapon->GetSocketLocation(ICombatInterface::Execute_GetWeaponTipSocketName(OwnerCharacter));
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				MouseHitLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HR,
				true
			);
			if (HR.bBlockingHit)
			{
				MouseHitLocation = HR.ImpactPoint;
				MouseHitActor = HR.GetActor();
			}
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(bool bIncludeFriendlyActors, TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);

	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		ActorsToIgnore,
		ChainRadius.GetValueAtLevel(GetAbilityLevel()),
		MouseHitActor->GetActorLocation(),
		OverlappingActors
	);
	if (!bIncludeFriendlyActors)            // Do not chain on friendly targets
	{
		OverlappingActors.RemoveAll([this](const AActor* Target)
			{
				return !UAuraAbilitySystemLibrary::IsNotFriend(GetAvatarActorFromActorInfo(), Target);
			});
	}
	int32 NumAdditionalTargets = MaxShockTargets.AsInteger(GetAbilityLevel());

	OutAdditionalTargets = UAuraAbilitySystemLibrary::GetClosestTargets(MaxShockTargets.AsInteger(GetAbilityLevel()), OverlappingActors, MouseHitActor->GetActorLocation());
}
