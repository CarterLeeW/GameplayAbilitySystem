// Copyright Carter Wooton


#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTagToFind, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTagExact(AbilityTagToFind))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for Ability Tag [%s] on AbilityInfo [%s]"), *AbilityTagToFind.ToString(), *GetNameSafe(this));
	}

	return FAuraAbilityInfo();
}
