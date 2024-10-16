// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IHighlightInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void HighlightActor();
	UFUNCTION(BlueprintNativeEvent)
	void UnHighlightActor();

	/* This really doesn't belong in this interface */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OverrideMoveToLocation(FVector& OutDestination);
};
