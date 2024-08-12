// Copyright Carter Wooton


#include "Game/AuraGameLibrary.h"
#include "Game/AuraGameModeBase.h"
#include "Game/AuraGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "AuraLogChannels.h"

AAuraGameModeBase* UAuraGameLibrary::GetAuraGameMode(const UObject* WorldContextObject)
{
    AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    UE_LOG(LogAura, Warning, TEXT("AuraGameModeBase is null in %s"), __FUNCTION__);
    return AuraGM;
}

UAuraGameInstance* UAuraGameLibrary::GetAuraGameInstance(const UObject* WorldContextObject)
{
    UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(UGameplayStatics::GetGameMode(WorldContextObject)->GetGameInstance());
    UE_LOG(LogAura, Warning, TEXT("AuraGameInstance is null in %s"), __FUNCTION__);
    return AuraGI;
}
