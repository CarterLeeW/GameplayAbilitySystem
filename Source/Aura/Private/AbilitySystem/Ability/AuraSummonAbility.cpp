// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
    const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
    const float DeltaSpread = NumMinions > 1 ? SpawnSpread / (NumMinions - 1) : 1.f;

    const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
    const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
    TArray<FVector> SpawnLocations;
    
    for (int32 i = 0; i < NumMinions; i++)
    {
        FVector Direction;
        if (NumMinions == 1)
        {
            Direction = Forward;
        }
        else
        {
            Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
        }
        FVector ChosenSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
        // Ensure spawn point is on the ground
        FHitResult Hit;
        GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
        if (Hit.bBlockingHit)
        {
            ChosenSpawnLocation = Hit.ImpactPoint;
        }
        SpawnLocations.Add(ChosenSpawnLocation);
    }
    return SpawnLocations;
}
