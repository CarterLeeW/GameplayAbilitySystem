// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveObjectInterface.h"
#include "ActivatedFlamePillar.generated.h"

UCLASS()
class AURA_API AActivatedFlamePillar : public AActor, public ISaveObjectInterface
{
	GENERATED_BODY()
	
public:	
	AActivatedFlamePillar();

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bActivated = false;

	/* Save Object Interface */
	virtual void LoadActor_Implementation() override;
	/* End Save Object Interface */
protected:
	virtual void BeginPlay() override;

};
