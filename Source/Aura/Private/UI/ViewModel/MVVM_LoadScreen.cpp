// Copyright Carter Wooton


#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LoadScreenSaveGame.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameLibrary.h"

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
	LoadSlot_0->SlotIndex = 0;
	LoadSlot_1->SlotIndex = 1;
	LoadSlot_2->SlotIndex = 2;
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	// None of this should happen on the client. AuraGM will be null
	AAuraGameModeBase* AuraGM = UAuraGameLibrary::GetAuraGameMode(this);
	if (IsValid(AuraGM))
	{
		UMVVM_LoadSlot*& CurrSlot = LoadSlots[Slot];

		CurrSlot->SetMapName(AuraGM->StartingMapName);
		CurrSlot->SetPlayerName(EnteredName);
		CurrSlot->SetPlayerLevel(1);
		CurrSlot->SlotStatus = ESaveSlotStatus::Taken;
		CurrSlot->PlayerStartTag = AuraGM->DefaultPlayerStartTag;
		AuraGM->SaveSlotData(LoadSlots[Slot], Slot);
		LoadSlots[Slot]->InitializeSlot();

		UAuraGameInstance* AuraGI = UAuraGameLibrary::GetAuraGameInstance(this);
		AuraGI->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
		AuraGI->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
		AuraGI->PlayerStartTag = AuraGM->DefaultPlayerStartTag;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Blue, FString("Please switch to single player"));
		return;
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(ESaveSlotStatus::EnterName);
	LoadSlots[Slot]->EnableSelectSlotButton.Broadcast(true);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	// broadcast play and delete delegates
	SlotSelected.Broadcast();
	// Disable selected slot's button and enable all others
	for (const TPair<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSaveSlot(SelectedSlot, SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = ESaveSlotStatus::Vacant;
		SelectedSlot->InitializeSlot();
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	UAuraGameInstance* AuraGI = UAuraGameLibrary::GetAuraGameInstance(this);
	if (IsValid(SelectedSlot))
	{
		AuraGI->PlayerStartTag = SelectedSlot->PlayerStartTag;
		AuraGI->LoadSlotName = SelectedSlot->GetLoadSlotName();
		AuraGI->LoadSlotIndex = SelectedSlot->SlotIndex;

		UAuraGameLibrary::GetAuraGameMode(this)->TravelToMap(SelectedSlot);
	}
}

void UMVVM_LoadScreen::LoadData()
{
	// None of this should happen on the client. AuraGM will be null
	AAuraGameModeBase* AuraGM = UAuraGameLibrary::GetAuraGameMode(this);
	if (IsValid(AuraGM))
	{
		for (TPair<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
		{
			ULoadScreenSaveGame* SaveObject = AuraGM->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
			UMVVM_LoadSlot*& CurrSlot = LoadSlot.Value;
		
			CurrSlot->SlotStatus = SaveObject->SaveSlotStatus;
			CurrSlot->SetPlayerName(SaveObject->PlayerName);
			CurrSlot->SetMapName(SaveObject->CurrentMapName);
			CurrSlot->PlayerStartTag = SaveObject->PlayerStartTag;
			CurrSlot->SetPlayerLevel(SaveObject->PlayerLevel);

			CurrSlot->InitializeSlot();
		}
	}
	else
	{
		return;
	}
}
