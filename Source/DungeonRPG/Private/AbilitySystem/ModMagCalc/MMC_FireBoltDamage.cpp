// DungeonRPG


#include "AbilitySystem/ModMagCalc/MMC_FireBoltDamage.h"

#include "AbilitySystem/RPGAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_FireBoltDamage::UMMC_FireBoltDamage()
{
}

float UMMC_FireBoltDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer *SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer *TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	const int32 Level = Spec.GetLevel();
	
	return 10 + 2 * Level;
}
