// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

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
        const FVector ChosenSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
        SpawnLocations.Add(ChosenSpawnLocation);
        DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 15.f, 12, FColor::Red, false, 1.f);
        UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Green, 1.f);
    }


    //UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FLinearColor::Red, 1.f);
    return SpawnLocations;
}
