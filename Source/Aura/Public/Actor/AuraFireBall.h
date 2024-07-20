// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraAbilityTypes.h"
#include "AuraFireBall.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;

	void ExplodeAndDealDamage();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnExplosionEffects();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(BlueprintReadWrite)
	bool bIsReturning = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DistanceToOwnerBeforeExploding = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDamageFriendlyCharacters = false;
};
