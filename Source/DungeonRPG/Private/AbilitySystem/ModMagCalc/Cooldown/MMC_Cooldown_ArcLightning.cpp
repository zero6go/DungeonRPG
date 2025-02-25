// DungeonRPG


#include "AbilitySystem/ModMagCalc/Cooldown/MMC_Cooldown_ArcLightning.h"

#include "AbilitySystem/RPGAttributeSet.h"

UMMC_Cooldown_ArcLightning::UMMC_Cooldown_ArcLightning()
{
	CooldownDef.AttributeToCapture = URPGAttributeSet::GetIntelligenceAttribute();
	CooldownDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CooldownDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(CooldownDef);
}

float UMMC_Cooldown_ArcLightning::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer *SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer *TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float CooldownReduction= 0.f;
	GetCapturedAttributeMagnitude(CooldownDef, Spec, EvaluateParameters, CooldownReduction);
	CooldownReduction = FMath::Max(CooldownReduction, 0.f);

	float CooldownBase = 9.f;
	int32 Level = Spec.GetLevel();
	switch (Level)
	{
	case 1 : CooldownBase = 9.f; break;
	case 2 : CooldownBase = 8.f; break;
	case 3 : CooldownBase = 7.f; break;
	case 4 : CooldownBase = 6.f; break;
	case 5 : CooldownBase = 5.f; break;
	case 6 : CooldownBase = 4.f; break;
	default : break;
	}

	return CooldownBase * (100.f - CooldownReduction) / 100.f;
}
