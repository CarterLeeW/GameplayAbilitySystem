// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveObjectInterface.h"
#include "ActivatedGate.generated.h"

UCLASS()
class AURA_API AActivatedGate : public AActor, public ISaveObjectInterface
{
	GENERATED_BODY()
	
public:	
	AActivatedGate();

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bOpen = false;

	/* SaveObjectInterface */
	virtual bool ShouldLoadTransform_Implementation() const override { return true; } // Gate may be in opened or closed position
	virtual void LoadActor_Implementation() override;
	/* End SaveObjectInterface */

protected:
	virtual void BeginPlay() override;

};
