// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraLifeSiphon.h"

FString UAuraLifeSiphon::GetDescription(int32 Level) const
{
	return FString::Printf(TEXT(
		// Title
		"<Title>LIFE SIPHON</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Passive</>\n"
		// Description
		"<Default>Defeating enemies restores %d percent of Aura's health</>\n\n"),
		Level, PercentOfHealthRestored.AsInteger(Level));
}

FString UAuraLifeSiphon::GetNextLevelDescription(int32 Level) const
{
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Passive</>\n"
		// Description
		"<Default>Defeating enemies restores %d percent of Aura's health</>\n\n"),
		Level+1, PercentOfHealthRestored.AsInteger(Level+1));
}
