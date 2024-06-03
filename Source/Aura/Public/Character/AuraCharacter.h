// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
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

	// CombatInterface
	//virtual int32 GetPlayerLevel() const override;
	virtual int32 GetPlayerLevel_Implementation() const override;
	// End CombatInterface

	// Player Interface
	virtual void AddToExp_Implementation(int32 InExp) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetExp_Implementation() const override;
	virtual int32 FindLevelForExp_Implementation(int32 InExp) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	// End Player Interface

	virtual void InitializeDefaultAttributes() const override;

private:
	virtual void InitAbilityActorInfo() override;
};
