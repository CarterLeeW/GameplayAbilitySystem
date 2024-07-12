// Copyright Carter Wooton


#include "Actor/PointCollection.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h" 

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Point_0 = CreateDefaultSubobject<USceneComponent>("Point_0");
	ImmutablePoints.Add(Point_0);
	SetRootComponent(Point_0);

	Point_1 = CreateDefaultSubobject<USceneComponent>("Point_1");
	ImmutablePoints.Add(Point_1);
	Point_1->SetupAttachment(RootComponent);

	Point_2 = CreateDefaultSubobject<USceneComponent>("Point_2");
	ImmutablePoints.Add(Point_2);
	Point_2->SetupAttachment(RootComponent);

	Point_3 = CreateDefaultSubobject<USceneComponent>("Point_3");
	ImmutablePoints.Add(Point_3);
	Point_3->SetupAttachment(RootComponent);

	Point_4 = CreateDefaultSubobject<USceneComponent>("Point_4");
	ImmutablePoints.Add(Point_4);
	Point_4->SetupAttachment(RootComponent);

	Point_5 = CreateDefaultSubobject<USceneComponent>("Point_5");
	ImmutablePoints.Add(Point_5);
	Point_5->SetupAttachment(RootComponent);

	Point_6 = CreateDefaultSubobject<USceneComponent>("Point_6");
	ImmutablePoints.Add(Point_6);
	Point_6->SetupAttachment(RootComponent);

	Point_7 = CreateDefaultSubobject<USceneComponent>("Point_7");
	ImmutablePoints.Add(Point_7);
	Point_7->SetupAttachment(RootComponent);

	Point_8 = CreateDefaultSubobject<USceneComponent>("Point_8");
	ImmutablePoints.Add(Point_8);
	Point_8->SetupAttachment(RootComponent);

	Point_9 = CreateDefaultSubobject<USceneComponent>("Point_9");
	ImmutablePoints.Add(Point_9);
	Point_9->SetupAttachment(RootComponent);

	Point_10 = CreateDefaultSubobject<USceneComponent>("Point_10");
	ImmutablePoints.Add(Point_10);
	Point_10->SetupAttachment(RootComponent);
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride) const
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("NumPoints argument is greater than number of elements in ImmutablePoints"));

	TArray<USceneComponent*> ArrayCopy;

	for (USceneComponent* Pt : ImmutablePoints)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Pt != Point_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Point_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Point_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);

		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, TArray<AActor*>(), 1500.f, GetActorLocation(), IgnoreActors);

		FHitResult HR;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HR, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HR.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HR.ImpactNormal));

		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}
