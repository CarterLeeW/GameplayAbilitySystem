// Copyright Carter Wooton


#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/SaveGame.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	// Delete current saved game in slot if it exists
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->PlayerName;

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}
