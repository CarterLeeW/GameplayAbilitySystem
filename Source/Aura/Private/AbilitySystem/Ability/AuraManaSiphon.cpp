// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraManaSiphon.h"

FString UAuraManaSiphon::GetDescription(int32 Level) const
{
	return FString::Printf(TEXT(
		// Title
		"<Title>MANA SIPHON</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Passive</>\n"
		// Description
		"<Default>Defeating enemies restores %d percent of Aura's mana</>\n\n"),
		Level, PercentOfManaRestored.AsInteger(Level));
}

FString UAuraManaSiphon::GetNextLevelDescription(int32 Level) const
{
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Passive</>\n"
		// Description
		"<Default>Defeating enemies restores %d percent of Aura's mana</>\n\n"),
		Level+1, PercentOfManaRestored.AsInteger(Level+1));
}
