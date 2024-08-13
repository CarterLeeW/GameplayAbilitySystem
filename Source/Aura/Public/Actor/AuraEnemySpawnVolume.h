// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveObjectInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class UBoxComponent;
class AAuraEnemySpawnPoint;

UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public ISaveObjectInterface
{
	GENERATED_BODY()
	
public:	
	AAuraEnemySpawnVolume();

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;
	/* Save Object Interface */
	virtual void LoadActor_Implementation() override;
	/* End Save Object Interface */
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
	TArray<AAuraEnemySpawnPoint*> SpawnPoints;

};
