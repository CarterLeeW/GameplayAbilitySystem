// Copyright Carter Wooton


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadCastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnhealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxhealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}
