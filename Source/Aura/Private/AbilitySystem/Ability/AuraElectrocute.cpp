// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraElectrocute.h"

FString UAuraElectrocute::GetDescription(int32 Level) const
{
	const float ManaCostPerSecond = DamagePeriod > 0.f ? FMath::Abs(GetManaCost(Level) / DamagePeriod) : 0.f;
	const float Cooldown = GetCooldown(Level);
	const float DamagePerSecond = DamagePeriod > 0.f ? Damage.GetValueAtLevel(Level) / DamagePeriod : 0.f;
	return FString::Printf(TEXT(
		// Title
		"<Title>ELECTROCUTE</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</><Small>/s</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Creates a shock chain, dealing </>"
		"<Damage>%d </>"
		"<Default>lightning damage per second to a maximum of %d targets with a chance to stun</>\n\n"),
		Level, ManaCostPerSecond, Cooldown, FMath::RoundToInt(DamagePerSecond), MaxShockTargets.AsInteger(Level));
}

FString UAuraElectrocute::GetNextLevelDescription(int32 Level) const
{
	const float ManaCostPerSecond = DamagePeriod > 0.f ? FMath::Abs(GetManaCost(Level+1) / DamagePeriod) : 0.f;
	const float Cooldown = GetCooldown(Level+1);
	const float DamagePerSecond = DamagePeriod > 0.f ? Damage.GetValueAtLevel(Level+1) / DamagePeriod : 0.f;
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</><Small>/s</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Creates a shock chain, dealing </>"
		"<Damage>%d </>"
		"<Default>lightning damage per second to a maximum of %d targets with a chance to stun</>\n\n"),
		Level+1, ManaCostPerSecond, Cooldown, FMath::RoundToInt(DamagePerSecond), MaxShockTargets.AsInteger(Level+1));
}
