// DungeonRPG


#include "AbilitySystem/ModMagCalc/MMC_CooldownReduction.h"

#include "AbilitySystem/RPGAttributeSet.h"

UMMC_CooldownReduction::UMMC_CooldownReduction()
{
	AgilityDef.AttributeToCapture = URPGAttributeSet::GetAgilityAttribute();
	AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AgilityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AgilityDef);
}

float UMMC_CooldownReduction::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer *SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer *TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float Agility= 0.f;
	GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluateParameters, Agility);
	Agility = FMath::Max(Agility, 0.f);

	return 100.f * Agility / (100.f + Agility);
}
