// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RPGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	FGameplayTag AbilityTag;
};
