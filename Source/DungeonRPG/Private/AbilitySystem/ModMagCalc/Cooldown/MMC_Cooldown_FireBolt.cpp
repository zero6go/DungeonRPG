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

	float CooldownBase = 2.f;
	int32 Level = Spec.GetLevel();
	switch (Level)
	{
		case 1 : CooldownBase = 2.f; break;
		case 2 : CooldownBase = 1.8f; break;
		case 3 : CooldownBase = 1.6f; break;
		case 4 : CooldownBase = 1.4f; break;
		case 5 : CooldownBase = 1.2f; break;
		case 6 : CooldownBase = 1.f; break;
		default : break;
	}

	return CooldownBase * (100.f - CooldownReduction) / 100.f;
}
