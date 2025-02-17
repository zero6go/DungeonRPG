// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Cooldown_FireBolt.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API UMMC_Cooldown_FireBolt : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_Cooldown_FireBolt();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition CooldownDef;
};
