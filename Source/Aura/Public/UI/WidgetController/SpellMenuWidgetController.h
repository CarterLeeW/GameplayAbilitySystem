// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraGlobalDelegates.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

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

	UFUNCTION(BlueprintCallable)
	void SpendSpellPointButtonPressed(const FGameplayTag& AbilityTag);
	UFUNCTION(BlueprintCallable)
	void GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed(const FGameplayTag& AbilityTag);
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot);
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bWaitingForEquipSelection;

private:
	FGameplayTag SelectedSlot;
};
