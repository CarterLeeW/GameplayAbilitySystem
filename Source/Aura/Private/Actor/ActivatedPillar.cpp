// Copyright Carter Wooton


#include "Actor/ActivatedPillar.h"

AActivatedPillar::AActivatedPillar()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("PillarMesh");
	Mesh->SetupAttachment(RootComponent);

	MoveToComponent = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment(Mesh);
}

void AActivatedPillar::LoadActor_Implementation()
{
	// Handled in Blueprint
}

void AActivatedPillar::HighlightActor_Implementation()
{
	Mesh->SetRenderCustomDepth(true);
}

void AActivatedPillar::UnHighlightActor_Implementation()
{
	Mesh->SetRenderCustomDepth(false);
}

void AActivatedPillar::OverrideMoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination = MoveToComponent->GetComponentLocation();
}

void AActivatedPillar::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->SetCustomDepthStencilValue(static_cast<int32>(CustomDepthStencilColor));
	Mesh->MarkRenderStateDirty();
}
