// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// EnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// End EnemyInterface

	// CombatInterface
	virtual int32 GetPlayerLevel_Implementation() const override { return Level; };
	virtual void Die() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override { CombatTarget = InCombatTarget; }
	virtual AActor* GetCombatTarget_Implementation() const override { return CombatTarget; }
	// End CombatInterface

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	bool GetHitReacting() const { return bHitReacting; }
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<UWidgetComponent> HealthBar;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	float BaseWalkSpeed = 250.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	float LifeSpan = 5.f;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Class Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Level = 1;

	void BindCallbacksToDependencies();
	void BroadcastInitialValues();
};
