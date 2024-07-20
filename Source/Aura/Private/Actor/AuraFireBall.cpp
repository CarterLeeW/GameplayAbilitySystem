// Copyright Carter Wooton

#include "Actor/AuraFireBall.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"
#include "AuraAbilityTypes.h"

void AAuraFireBall::Tick(float DeltaSeconds)
{
	if (bIsReturning && IsValid(GetOwner()) && HasAuthority())
	{
		const bool bIsWithinExplodingDistance = (FVector::Distance(GetOwner()->GetActorLocation(), GetActorLocation())) <= DistanceToOwnerBeforeExploding;
		if (bIsWithinExplodingDistance)
		{
			SpawnExplosionEffects();
			ExplodeAndDealDamage();
		}
	}
}

void AAuraFireBall::ExplodeAndDealDamage()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	TArray<AActor*> ActorsToDamage;
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(
		this,
		ActorsToIgnore,
		DamageEffectParams.RadialDamageOuterRadius,
		GetActorLocation(),
		ActorsToDamage
	);
	// Remove friendly actors | friendly fire off
	if (!bDamageFriendlyCharacters)
	{
		ActorsToDamage.RemoveAll([this](const AActor* Target)
			{
				return !UAuraAbilitySystemLibrary::IsNotFriend(GetOwner(), Target);
			});
	}

	for (AActor* Actor : ActorsToDamage)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
		{
			// Apply gameplay effects here, now that ASC is valid
			DamageEffectParams.TargetAbilitySystemComponent = ASC;
			DamageEffectParams.RadialDamageOrigin = GetActorLocation();
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
	Destroy();
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraFireBall::Destroyed()
{

}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageEffectParams.SourceAbilitySystemComponent) return;
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return; // Do not hit yourself

	// This disables friendly fire
	if (SourceAvatarActor && !UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor))
	{
		return;
	}

	if (HasAuthority())
	{
		if (!bHit)
		{
			SpawnExplosionEffects();
			ExplodeAndDealDamage();
		}
		bHit = true;
	}
}
