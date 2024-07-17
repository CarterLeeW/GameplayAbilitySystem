// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float FireDamage = Damage.GetValueAtLevel(Level);
	return FString::Printf(TEXT(
		// Title
		"<Title>FIRE BLAST</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Launches %d balls of fire in a circle from the caster, exploding on impact and dealing </>"
		"<Damage>%d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
		Level, ManaCost, Cooldown, NumFireBalls, FMath::RoundToInt(FireDamage));
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level + 1));
	const float Cooldown = GetCooldown(Level + 1);
	const float FireDamage = Damage.GetValueAtLevel(Level + 1);
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Launches %d balls of firein a circle from the caster, exploding on impact and dealing </>"
		"<Damage>%d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
		Level + 1, ManaCost, Cooldown, NumFireBalls, FMath::RoundToInt(FireDamage));
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	return TArray<AAuraFireBall*>();
}
