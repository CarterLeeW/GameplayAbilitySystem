// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/)

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Delegate
	FOnPlayerStatChanged OnExpChangedDelegate;
	// Delegate
	FOnPlayerStatChanged OnLevelChangedDelegate;

	void SetLevel(int32 InLevel);
	void AddToLevel(int32 InLevel);
	int32 GetPlayerLevel() const { return Level; }
	void SetExp(int32 InExp);
	void AddToExp(int32 InExp);
	int32 GetExp() const { return Exp; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Exp)
	int32 Exp = 0;
	UFUNCTION()
	void OnRep_Exp(int32 OldExp);
};
