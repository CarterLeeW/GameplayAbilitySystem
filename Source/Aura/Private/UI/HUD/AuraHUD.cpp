// Copyright Carter Wooton


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystemComponent.h"

#define ACCESS_WIDGET_CONTROLLER(WC, WCC, Params) \
	if (!WC)\
	{\
		WC = NewObject<U##WC##>(this, WCC);\
		WC->SetWidgetControllerParams(Params);\
		WC->BindCallbacksToDependencies();\
		return WC;\
	}\
	else\
	{\
		return WC;\
	}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	ACCESS_WIDGET_CONTROLLER(OverlayWidgetController, OverlayWidgetControllerClass, WCParams);
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class is unitialized in BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class is unitialized in BP_AuraHUD"));
	

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	ACCESS_WIDGET_CONTROLLER(AttributeMenuWidgetController, AttributeMenuWidgetControllerClass, WCParams);
}
