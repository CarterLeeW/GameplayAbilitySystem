// Copyright Carter Wooton


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTagToFind, bool bLogNotFound) const
{
	for (const auto& AIAction : AbilityInputActions)
	{
		if (AIAction.InputAction && (AIAction.InputTag == InputTagToFind))
		{
			return AIAction.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTagToFind.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
