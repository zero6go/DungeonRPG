// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

struct FAbilityInfo;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityInputPressed(const FGameplayTag &InputTag);
	void AbilityInputHeld(const FGameplayTag &InputTag);
	void AbilityInputReleased(const FGameplayTag &InputTag);
};
