// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraGameLibrary.generated.h"

class AAuraGameModeBase;
class UAuraGameInstance;

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "AuraGameLibrary|GameMode", meta = (WorldContext = "WorldContextObject"))
	static AAuraGameModeBase* GetAuraGameMode(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "AuraGameLibrary|GameInstance", meta = (WorldContext = "WorldContextObject"))
	static UAuraGameInstance* GetAuraGameInstance(const UObject* WorldContextObject);
};
