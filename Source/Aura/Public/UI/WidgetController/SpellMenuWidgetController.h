// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraGlobalDelegates.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

USTRUCT(BlueprintType)
struct FSelectedAbility
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag Ability = FGameplayTag();
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|PlayerStats")
	FPlayerStatChangedSignature OnPlayerSpellPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "Gas|Ability")
	FWaitForEquipSelectionSignature WaitForEquip;

	UFUNCTION(BlueprintCallable)
	void SpendSpellPointButtonPressed(const FGameplayTag& AbilityTag);
	UFUNCTION(BlueprintCallable)
	void GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UPROPERTY(BlueprintReadWrite)
	FSelectedAbility SelectedAbility;

private:
	bool bWaitingForEquipSelection = false;
};
