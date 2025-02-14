// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAbilityGiven, const FGameplayTag&, AbilityTag, const FGameplayTag&, InputTag);

struct FAbilityClassAndLevel;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAbilityGiven AbilityGivenDelegate;
	
	void GivePlayerAbility(const FAbilityClassAndLevel &AbilityInfo);
	void GiveCharacterAbility(const FAbilityClassAndLevel &AbilityInfo);
	
	void AbilityInputPressed(const FGameplayTag &InputTag);
	void AbilityInputHeld(const FGameplayTag &InputTag);
	void AbilityInputReleased(const FGameplayTag &InputTag);

protected:
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_AbilityGiven(const FGameplayTag &AbilityTag, const FGameplayTag &InputTag);

private:
	bool bStartupAbilitiesGiven = false;
};
