// Copyright Carter Wooton


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForExp(int32 Exp) const
{
	/* LevelUpInformation[0] is just a placeholder. Useful information begins at index 1 */
	for (int32 i = 1; i < LevelUpInformation.Num(); i++)
	{
		if (Exp < LevelUpInformation[i].LevelUpRequirement)
		{
			return i;
		}
	}

	return 1;
}
