// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraFireBolt.h"
#include "Actor/AuraProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const FString Bolts = NumProjectiles.AsInteger(Level) == 1 ? TEXT("bolt") : TEXT("bolts");
	const float FireDamage = Damage.GetValueAtLevel(Level);
	return FString::Printf(TEXT(
		// Title
		"<Title>FIRE BOLT</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Launches %d %s of fire, exploding on impact and dealing </>"
		"<Damage>%d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
		Level, ManaCost, Cooldown, NumProjectiles.AsInteger(Level), *Bolts, FMath::RoundToInt(FireDamage));
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level+1));
	const float Cooldown = GetCooldown(Level+1);
	const FString Bolts = NumProjectiles.AsInteger(Level+1) + 1 == 1 ? TEXT("bolt") : TEXT("bolts");
	const float FireDamage = Damage.GetValueAtLevel(Level+1);
	return FString::Printf(TEXT(
	// Title
	"<Title>NEXT LEVEL</>\n\n"
	// Details
	"<Default>Level: %d</>\n"
	"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
	"<Small>Cooldown: </><Cooldown>%.1f</>\n"
	// Description
	"<Default>Launches %d %s of fire, exploding on impact and dealing </>"
	"<Damage>%d </>"
	"<Default>fire damage with a chance to burn</>\n\n"),
	Level, ManaCost, Cooldown, NumProjectiles.AsInteger(Level + 1), * Bolts, FMath::RoundToInt(FireDamage));
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
			GetAvatarActorFromActorInfo(),
			SocketTag
		);
		FVector Forward = (ProjectileTargetLocation - SocketLocation);
		Forward.Normalize();
		const float DeltaSpread = NumProjectiles.AsInteger(GetAbilityLevel()) > 1 ? SpawnSpread / (NumProjectiles.AsInteger(GetAbilityLevel()) - 1) : 1.f;
		const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
		const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
		TArray<FVector> SpawnLocations;

		for (int32 i = 0; i < NumProjectiles.AsInteger(GetAbilityLevel()); i++)
		{
			FVector Direction;
			if (NumProjectiles.AsInteger(GetAbilityLevel()) == 1)
			{
				Direction = Forward;
			}
			else
			{
				Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			}
			FVector ChosenSpawnLocation = SocketLocation + Direction;
			FRotator Rotation = Direction.Rotation();
			//Rotation.Pitch = 0.f;

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(ChosenSpawnLocation);
			SpawnTransform.SetRotation(Rotation.Quaternion());

			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), ChosenSpawnLocation, ChosenSpawnLocation + Rotation.Vector() * 100.f, 5, FLinearColor::Blue, 2.f, 5.f);
		}
		
		
	}
}
