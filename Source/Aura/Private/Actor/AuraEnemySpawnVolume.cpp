// Copyright Carter Wooton


#include "Actor/AuraEnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Actor/AuraEnemySpawnPoint.h"

// Sets default values
AAuraEnemySpawnVolume::AAuraEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	RootComponent = Box;
}

void AAuraEnemySpawnVolume::LoadActor_Implementation()
{
	// Don't spawn anything
	if (bReached)
	{
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AAuraEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &AAuraEnemySpawnVolume::OnBoxOverlap);
}

void AAuraEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		for (AAuraEnemySpawnPoint* SpawnPoint : SpawnPoints)
		{
			if (IsValid(SpawnPoint))
			{
				SpawnPoint->SpawnEnemy();
			}
		}
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
