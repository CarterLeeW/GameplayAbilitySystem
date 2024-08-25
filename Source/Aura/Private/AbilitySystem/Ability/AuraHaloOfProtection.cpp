// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraHaloOfProtection.h"

FString UAuraHaloOfProtection::GetDescription(int32 Level) const
{
	return FString::Printf(TEXT(
		// Title
		"<Title>AURA OF PROTECTION</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Passive</>\n"
		// Description
		"<Default>Aura receives %d percent less damage from enemies</>\n\n"),
		Level, PercentOfDamageDeflected.AsInteger(Level));
}

FString UAuraHaloOfProtection::GetNextLevelDescription(int32 Level) const
{
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Passive</>\n"
		// Description
		"<Default>Aura receives %d percent less damage from enemies</>\n\n"),
		Level+1, PercentOfDamageDeflected.AsInteger(Level+1));
}
