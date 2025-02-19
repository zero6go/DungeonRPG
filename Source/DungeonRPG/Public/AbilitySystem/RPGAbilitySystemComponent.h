// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FAbilityGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityStatusChanged, const FGameplayAbilitySpec&);

struct FAbilityClassAndLevel;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FAbilityGiven AbilityGivenDelegate;
	FAbilityStatusChanged AbilityStatusChangedDelegate;
	
	bool bStartupAbilitiesGiven = false;
	
	void GivePlayerAbility(const FAbilityClassAndLevel &AbilityInfo);
	void GiveCharacterAbility(const FAbilityClassAndLevel &AbilityInfo);
	
	void AbilityInputPressed(const FGameplayTag &InputTag);
	void AbilityInputHeld(const FGameplayTag &InputTag);
	void AbilityInputReleased(const FGameplayTag &InputTag);

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static int32 GetAbilityLevelFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayAbilitySpec* GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetAbilitySpecFromInputTag(const FGameplayTag& InputTag);

	void EquipSpell(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);

	void UpdateAbilityStatusUnlockable(int32 OldLevel, int32 NewLevel);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSpellLevelUp(const FGameplayTag& AbilityTag);

	bool GetDescription(const FGameplayTag &AbilityTag, const int32 AbilityLevel, FString &Description, FString &NextLevelDescription);
	
protected:
	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayAbilitySpec& AbilitySpec);
private:
	FGameplayTag EquippedAbilityTag;
};
