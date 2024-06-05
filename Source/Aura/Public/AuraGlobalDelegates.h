// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AuraGlobalDelegates.generated.h"

// Player Stat Delegate
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerStatChangedSignature, int32, StatValue);

/**
 * 
 */
class AURA_API AuraGlobalDelegates
{
public:
	AuraGlobalDelegates();
	~AuraGlobalDelegates();
};
