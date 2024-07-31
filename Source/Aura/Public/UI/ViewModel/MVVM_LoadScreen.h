// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	void InitializeLoadSlots();
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	void LoadData();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	// Vacant
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	// New Game
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	// Load Game
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot;
};
