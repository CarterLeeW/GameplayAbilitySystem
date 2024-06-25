// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PrevSlot*/);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();
	/* Delegates */
	FEffectAssetTagsSignature EffectAssetTagsDelegate;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged OnAbilityStatusChanged;
	FAbilityEquipped OnAbilityEquipped;

	bool bStartupAbilitiesGiven = false;

	void ForEachAbility(const FForEachAbility& Delegate);
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilitiesInSlot(const FGameplayTag& Slot);
	static bool AbilityHasSlot(const FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	void UpdateAbilityStatus(int32 Level);
	UFUNCTION(BlueprintCallable)
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	UFUNCTION(Server, Reliable)
	void Server_UpgradeAttribute(const FGameplayTag& AttributeTag);
	UFUNCTION(Server, Reliable)
	void Server_SpendSpellPoint(const FGameplayTag& AbilityTag);
	UFUNCTION(Server, Reliable)
	void Server_EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);
protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	UFUNCTION(Client, Reliable)
	void Client_UpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
	UFUNCTION(Client, Reliable)
	void Client_EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot);
	virtual void OnRep_ActivateAbilities() override;
};
