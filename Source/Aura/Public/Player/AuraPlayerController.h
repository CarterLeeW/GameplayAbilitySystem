// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;
class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;

USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	const AActor* Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsOccluded;
};

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	FHitResult GetCursorHitResult() const { return CursorHit; }

	// Damage widget
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(ACharacter* TargetCharacter, float DamageAmount, bool bBlockedHit, bool bCriticalHit);

	/* Occlusion */
	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BeginOcclusion(const FCameraOccludedActor& OccludedActor) const;
	/* End Occlusion */

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/* Occlusion */
	/** How much of the Pawn capsule Radius and Height
   * should be used for the Line Trace before considering an Actor occluded?
   * Values too low may make the camera clip through walls.
   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Occlusion",
		meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float CapsulePercentageForTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Materials")
	TObjectPtr<UMaterialInterface> FadeMaterial;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
	TObjectPtr<USpringArmComponent> ActiveSpringArm;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
	TObjectPtr<UCameraComponent> ActiveCamera;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
	TObjectPtr<UCapsuleComponent> ActiveCapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion")
	bool IsOcclusionEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Occlusion")
	bool DebugLineTraces;
	/* End Occlusion */

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	void Move(const FInputActionValue& InputActionValue);

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	void ShiftPressed() { bShiftKeyHeld = true; };
	void ShiftReleased() { bShiftKeyHeld = false; };
	bool bShiftKeyHeld = false;

	// Highlighting
	void CursorTrace();
	FHitResult CursorHit;
	IEnemyInterface* LastTargetActor;
	IEnemyInterface* ThisTargetActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	// Movement
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.0f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.0f;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	// Damage widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	/* Occlusion */
	TMap<const AActor*, FCameraOccludedActor> OccludedActors;

	bool HideOccludedActor(const AActor* Actor);
	bool OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ShowOccludedActor(FCameraOccludedActor& OccludedActor);
	bool OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ForceShowOccludedActors();

	FORCEINLINE bool ShouldCheckCameraOcclusion() const
	{
		return IsOcclusionEnabled && FadeMaterial && ActiveCamera && ActiveCapsuleComponent;
	}
	/* End Occlusion */
};
