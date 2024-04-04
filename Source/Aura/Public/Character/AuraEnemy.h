// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// EnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// End EnemyInterface

	// CombatInterface
	virtual int32 GetPlayerLevel() const override { return Level; };
	// End CombatInterface

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChanged;

	bool GetHitReacting() const { return bHitReacting; }
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Character Class Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Level = 1;

	void BindCallbacksToDependencies();
	void BroadcastInitialValues();
};
