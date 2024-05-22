// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraSummonAbility.h"
#include "Character/AuraCharacterBase.h"

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

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass() const
{
    const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
    return MinionClasses[Selection];
}

APawn* UAuraSummonAbility::SummonMinion(TSubclassOf<AActor> Class, const FVector& Location, const FRotator& Rotation, AActor* Owner)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;
    AAuraCharacterBase* OwningEnemy = Cast<AAuraCharacterBase>(Owner);
    
    APawn* Minion = GetWorld()->SpawnActor<APawn>(Class, Location, Rotation, SpawnParams);
    Minion->OnDestroyed.AddDynamic(OwningEnemy, &AAuraCharacterBase::MinionHasDied);
    return Minion;
}
