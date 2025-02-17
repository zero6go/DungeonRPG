// DungeonRPG


#include "AbilitySystem/ModMagCalc/Cooldown/MMC_Cooldown_FireBolt.h"

#include "AbilitySystem/RPGAttributeSet.h"

UMMC_Cooldown_FireBolt::UMMC_Cooldown_FireBolt()
{
	CooldownDef.AttributeToCapture = URPGAttributeSet::GetIntelligenceAttribute();
	CooldownDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CooldownDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(CooldownDef);
}

float UMMC_Cooldown_FireBolt::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer *SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer *TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float CooldownReduction= 0.f;
	GetCapturedAttributeMagnitude(CooldownDef, Spec, EvaluateParameters, CooldownReduction);
	CooldownReduction = FMath::Max(CooldownReduction, 0.f);

	return 1.f * (100.f - CooldownReduction) / 100.f;
}
