// Copyright Carter Wooton


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	// TODO: check slot status based on loaded data
	SetWidgetSwitcherIndex.Broadcast(2);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}