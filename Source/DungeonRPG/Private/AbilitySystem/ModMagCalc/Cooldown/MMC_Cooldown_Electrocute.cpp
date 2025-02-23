// DungeonRPG


#include "AbilitySystem/ModMagCalc/Cooldown/MMC_Cooldown_Electrocute.h"

#include "AbilitySystem/RPGAttributeSet.h"

UMMC_Cooldown_Electrocute::UMMC_Cooldown_Electrocute()
{
	CooldownDef.AttributeToCapture = URPGAttributeSet::GetIntelligenceAttribute();
	CooldownDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CooldownDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(CooldownDef);
}

float UMMC_Cooldown_Electrocute::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer *SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer *TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float CooldownReduction= 0.f;
	GetCapturedAttributeMagnitude(CooldownDef, Spec, EvaluateParameters, CooldownReduction);
	CooldownReduction = FMath::Max(CooldownReduction, 0.f);

	float CooldownBase = 5.f;
	int32 Level = Spec.GetLevel();
	switch (Level)
	{
	case 1 : CooldownBase = 5.f; break;
	case 2 : CooldownBase = 4.5f; break;
	case 3 : CooldownBase = 4.f; break;
	case 4 : CooldownBase = 3.5f; break;
	case 5 : CooldownBase = 3.f; break;
	case 6 : CooldownBase = 2.5f; break;
	default : break;
	}

	return CooldownBase * (100.f - CooldownReduction) / 100.f;
}
