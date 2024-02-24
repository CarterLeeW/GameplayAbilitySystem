// Copyright Carter Wooton


#include "Character/AuraEnemy.h"
#include "DrawDebugHelpers.h"

void AAuraEnemy::HighlightActor()
{
	bIsHighlighted = true;
}

void AAuraEnemy::UnHighlightActor()
{
	bIsHighlighted = false;
}
