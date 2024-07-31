// Copyright Carter Wooton


#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LoadScreenSaveGame.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlots.Add(2, LoadSlot_2);
	LoadSlot_0->SetLoadSlotName("LoadSlot_0");
	LoadSlot_1->SetLoadSlotName("LoadSlot_1");
	LoadSlot_2->SetLoadSlotName("LoadSlot_2");
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SlotStatus = ESaveSlotStatus::ESSS_Taken;
	AuraGM->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(ESaveSlotStatus::ESSS_EnterName);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{

}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TPair<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuraGM->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		LoadSlot.Value->SlotStatus = SaveObject->SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(SaveObject->PlayerName);
		LoadSlot.Value->InitializeSlot();
	}
}
