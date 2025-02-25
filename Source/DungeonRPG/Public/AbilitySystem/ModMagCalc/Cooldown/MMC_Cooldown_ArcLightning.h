// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Cooldown_ArcLightning.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API UMMC_Cooldown_ArcLightning : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_Cooldown_ArcLightning();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition CooldownDef;
};
