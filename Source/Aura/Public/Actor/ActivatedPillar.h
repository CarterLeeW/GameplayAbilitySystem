// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveObjectInterface.h"
#include "Interaction/HighlightInterface.h"
#include "Aura/Aura.h"
#include "ActivatedPillar.generated.h"

UCLASS()
class AURA_API AActivatedPillar : public AActor, public ISaveObjectInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:	
	AActivatedPillar();

	UPROPERTY(EditAnywhere)
	FCustomDepthStencilColor CustomDepthStencilColor = FCustomDepthStencilColor::Tan;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;
	/* Save Object Interface */
	virtual void LoadActor_Implementation() override;
	/* End Save Object Interface */

	/* Highlight Interface */
	virtual void HighlightActor_Implementation();
	virtual void UnHighlightActor_Implementation();
	virtual void OverrideMoveToLocation_Implementation(FVector& OutDestination) override;
	/* End Highlight Interface */

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;
};
