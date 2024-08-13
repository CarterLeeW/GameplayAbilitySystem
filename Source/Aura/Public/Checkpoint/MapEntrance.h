// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveObjectInterface.h"
#include "Interaction/HighlightInterface.h"
#include "Aura/Aura.h"
#include "MapEntrance.generated.h"

class USphereComponent;

UCLASS()
class AURA_API AMapEntrance : public AActor, public ISaveObjectInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:	
	AMapEntrance();

	UPROPERTY(EditAnywhere)
	FCustomDepthStencilColor CustomDepthStencilColor = FCustomDepthStencilColor::Blue;
	/* Highlight Interface */
	virtual void HighlightActor_Implementation();
	virtual void UnHighlightActor_Implementation();
	virtual void OverrideMoveToLocation_Implementation(FVector& OutDestination) override;
	/* End Highlight Interface */

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;
};
