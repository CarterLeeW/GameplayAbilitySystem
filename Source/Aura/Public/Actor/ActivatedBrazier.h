// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveObjectInterface.h"
#include "ActivatedBrazier.generated.h"

UCLASS()
class AURA_API AActivatedBrazier : public AActor, public ISaveObjectInterface
{
	GENERATED_BODY()
	
public:	
	AActivatedBrazier();

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bFlame_0_On= false;
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bFlame_1_On = false;
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bFlame_2_On = false;
	/* Save Object Interface */
	virtual void LoadActor_Implementation() override;
	/* End Save Object Interface */

protected:
	virtual void BeginPlay() override;

};
