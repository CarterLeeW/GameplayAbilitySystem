// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAILibrary.generated.h"

USTRUCT(BlueprintType)
struct FActorFloatPair
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	AActor* Actor = nullptr;
	UPROPERTY(BlueprintReadOnly)
	float Value = 0.f;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "AuraAILibrary|BehaviorTreeService", meta = (WorldContext = "WorldContextObject"))
	static FActorFloatPair GetClosestActor(const UObject* WorldContextObject, const APawn* OwningPawn, const TArray<AActor*>& Actors);
};
