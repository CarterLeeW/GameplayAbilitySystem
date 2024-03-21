// Copyright Carter Wooton


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadCastInitialValues()
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get()->Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
