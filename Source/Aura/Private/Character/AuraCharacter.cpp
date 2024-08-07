// Copyright Carter Wooton


#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "UI/HUD/AuraHUD.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Game/AuraGameLibrary.h"
#include "Game/LoadScreenSaveGame.h"
#include "Game/AuraGameModeBase.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoomComponent = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoomComponent->SetupAttachment(GetRootComponent());
	CameraBoomComponent->SetUsingAbsoluteRotation(true);
	CameraBoomComponent->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoomComponent, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	// Load player data from disk
	LoadProgress();
}

void AAuraCharacter::LoadProgress()
{
	if (AAuraGameModeBase* AuraGM = UAuraGameLibrary::GetAuraGameMode(this))
	{
		if (ULoadScreenSaveGame* SaveData = AuraGM->RetrieveInGameSaveData())
		{
			if (SaveData->bFirstTimeLoadIn) // Use default attributes
			{
				InitializeDefaultAttributes();
				AddCharacterAbilities();
			}
			else                            // Load from disk
			{
				// TODO: Load Abilities from disk
				if (AAuraPlayerState* AuraPS = Cast<AAuraPlayerState>(GetPlayerState()))
				{
					AuraPS->SetLevel(SaveData->PlayerLevel);
					AuraPS->SetExp(SaveData->Exp);
					AuraPS->SetAttributePoints(SaveData->AttributePoints);
					AuraPS->SetSpellPoints(SaveData->SpellPoints);
				}
				UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
			}
		}
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
	/* The following fix is in response to issues in 241 */
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!AuraASC->bStartupAbilitiesGiven)
	{
		AuraASC->bStartupAbilitiesGiven = true;
		AuraASC->AbilitiesGivenDelegate.Broadcast();
	}
}

int32 AAuraCharacter::GetPlayerLevel_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MulticastHandleDeath(DeathImpulse);
}

void AAuraCharacter::AddToExp_Implementation(int32 InExp)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToExp(InExp);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 AAuraCharacter::GetExp_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetExp();
}

int32 AAuraCharacter::FindLevelForExp_Implementation(int32 InExp) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForExp(InExp);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InPlayerLevel);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatus(AuraPlayerState->GetPlayerLevel());
	}

}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	if (AAuraGameModeBase* AuraGM = UAuraGameLibrary::GetAuraGameMode(this))
	{
		if (ULoadScreenSaveGame* SaveData = AuraGM->RetrieveInGameSaveData())
		{
			// Save all persistent data
			SaveData->PlayerStartTag = CheckpointTag;
			if (AAuraPlayerState* AuraPS = Cast<AAuraPlayerState>(GetPlayerState()))
			{
				SaveData->PlayerLevel = AuraPS->GetPlayerLevel();
				SaveData->Exp = AuraPS->GetExp();
				SaveData->AttributePoints = AuraPS->GetAttributePoints();
				SaveData->SpellPoints = AuraPS->GetSpellPoints();
			}
			SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
			SaveData->Inteligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
			SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
			SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

			SaveData->bFirstTimeLoadIn = false;

			if (HasAuthority())
			{
				FForEachAbility SaveAbilityDelegate;
				UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
				SaveAbilityDelegate.BindLambda(
					[this, AuraASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
					{
						FSavedAbility SavedAbility;
						FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec(AbilitySpec);
						UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
						FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

						SavedAbility.GameplayAbility = Info.Ability;
						SavedAbility.AbilityLevel = AbilitySpec.Level;
						SavedAbility.AbilitySlot = AuraASC->GetSlotFromAbilityTag(AbilityTag);
						SavedAbility.AbilityStatus = AuraASC->GetStatusFromAbilityTag(AbilityTag);
						SavedAbility.AbilityTag = AbilityTag;
						SavedAbility.AbilityType = Info.AbilityType;

						SaveData->SavedAbilities.Add(SavedAbility);
					}
				);
				AuraASC->ForEachAbility(SaveAbilityDelegate);
			}

			AuraGM->SaveInGameProgressData(SaveData);
		}
	}
}

void AAuraCharacter::InitializeDefaultAttributes() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	ApplyEffectToSelf(DefaultPrimaryAttributes, AuraPlayerState->GetPlayerLevel());
	ApplyEffectToSelf(DefaultSecondaryAttributes, AuraPlayerState->GetPlayerLevel());
	ApplyEffectToSelf(DefaultVitalAttributes, AuraPlayerState->GetPlayerLevel());
}

void AAuraCharacter::OnRep_Stunned()
{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(FAuraGameplayTags::Get()->Player_Block_CursorTrace);
		BlockedTags.AddTag(FAuraGameplayTags::Get()->Player_Block_InputHeld);
		BlockedTags.AddTag(FAuraGameplayTags::Get()->Player_Block_InputPressed);
		BlockedTags.AddTag(FAuraGameplayTags::Get()->Player_Block_InputReleased);
		if (bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
	OnStunTagChanged.Broadcast(bIsStunned);
}

void AAuraCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get()->Debuff_Stun, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AAuraCharacter::StunTagChanged
	);

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// Set Default Attributes

}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}
