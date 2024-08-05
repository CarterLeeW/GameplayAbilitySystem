// Copyright Carter Wooton


#include "Game/AuraGameLibrary.h"
#include "Game/AuraGameModeBase.h"
#include "Game/AuraGameInstance.h"
#include "Kismet/GameplayStatics.h"

AAuraGameModeBase* UAuraGameLibrary::GetAuraGameMode(const UObject* WorldContextObject)
{
    AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    checkf(AuraGM, TEXT("%s is null"), *AuraGM->GetName());
    return AuraGM;
}

UAuraGameInstance* UAuraGameLibrary::GetAuraGameInstance(const UObject* WorldContextObject)
{
    UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(UGameplayStatics::GetGameMode(WorldContextObject)->GetGameInstance());
    checkf(AuraGI, TEXT("%s is null"), *AuraGI->GetName());
    return AuraGI;
}
