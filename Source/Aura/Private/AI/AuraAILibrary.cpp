// Copyright Carter Wooton


#include "AI/AuraAILibrary.h"

FActorFloatPair UAuraAILibrary::GetClosestActor(const UObject* WorldContextObject, const APawn* OwningPawn, const TArray<AActor*>& Actors)
{
	FActorFloatPair ADPair;
	AActor* ClosestActor = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();
	for (const auto& Actor : Actors)
	{
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	ADPair.Actor = ClosestActor;
	ADPair.Value = ClosestDistance;

	return ADPair;
}
