// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	// AActor Interface
	virtual void PossessedBy(AController* NewController) override;
	// End AActor Interface

	// APawn Interface
	virtual void OnRep_PlayerState() override;
	//End APawn Interface

private:
	virtual void InitAbilityActorInfo() override;
};
