// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_FireBoltDamage.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API UMMC_FireBoltDamage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_FireBoltDamage();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
};
