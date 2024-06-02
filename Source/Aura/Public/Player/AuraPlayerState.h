// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

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
	UFUNCTION(BlueprintCallable)
	int32 GetPlayerLevel() const { return Level; }
	void SetExp(int32 InExp);
	UFUNCTION(BlueprintCallable)
	void AddToExp(int32 InExp);
	UFUNCTION(BlueprintCallable)
	int32 GetExp() const { return Exp; }

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Level, meta = (ClampMin = "1"))
	int32 Level = 1;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Exp)
	int32 Exp = 0;
	UFUNCTION()
	void OnRep_Exp(int32 OldExp);
};
