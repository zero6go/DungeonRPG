// DungeonRPG


#include "AbilitySystem/Abilities/RPGGameplayAbility.h"

#include "GameplayModMagnitudeCalculation.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Engine/CurveTable.h"

FString URPGGameplayAbility::GetDescription(int32 Level)
{
	if (Level > 6) return FString::Printf(TEXT("<Default>已满级</>"));

	float Damage = 0.f;
	if (DamageCurveTable) Damage = DamageCurveTable->FindCurve("Damage", "")->Eval(Level);
	float ManaCost = -GetAbilityManaCost(Level);
	
	FString Description = FString::Printf(TEXT("<Title>%s</>\n\n<Default>%s</>\n"), *AbilityName.ToString(), *AbilityDescription.ToString());
	FString LevelDescription = FString::Printf(TEXT("<Default>等级： </><Level>%d</>\n"), Level);
	FString DamageDescription = FString::Printf(TEXT("<Default>伤害： </><Damage>%.1f</>\n"), Damage);
	FString ManaCostDescription = FString::Printf(TEXT("<Default>魔法消耗： </><ManaCost>%.1f</>\n"), ManaCost);
	FString CooldownDescription = FString::Printf(TEXT("<Default>冷却时间： </><Cooldown>%.1f</>\n"), CooldownAccordingToMMC[Level-1]);
	
	return Description + LevelDescription + DamageDescription + ManaCostDescription + CooldownDescription;
}

FString URPGGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>技能解锁等级： </><Level>%d</>"), Level);
}
float URPGGameplayAbility::GetAbilityManaCost(int32 Level) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect *ManaCostEffect = GetCostGameplayEffect())
	{
		for (auto Modifier : ManaCostEffect->Modifiers)
		{
			if (Modifier.Attribute == URPGAttributeSet::GetManaAttribute())
			{
				Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, ManaCost);
			}
		}
	}
	return ManaCost;
}
