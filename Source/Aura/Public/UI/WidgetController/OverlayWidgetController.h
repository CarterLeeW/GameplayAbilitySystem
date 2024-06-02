// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FGameplayAttributeData;
class UAuraUserWidget;
class UAuraAttributeSet;
class UAbilityInfo;
class UAuraAbilitySystemComponent;
struct FAuraAbilityInfo;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag MessageTag = FGameplayTag();
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Message = FText();
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UAuraUserWidget> MessageWidget;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);
// Widget Row Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
// Ability Info Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);
// Player Stat Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerExpSignature, int32, StatValue);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Ability")
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|PlayerStat")
	FPlayerExpSignature PlayerExpDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC);

	void OnExpChanged(int32 NewExp);
private:

};

template<typename T>
inline T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
