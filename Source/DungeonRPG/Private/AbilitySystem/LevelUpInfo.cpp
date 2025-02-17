// DungeonRPG


#include "AbilitySystem/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelByXP(int32 XP)
{
	for (int i = 0; i < LevelUpInfos.Num(); i++)
	{
		if(LevelUpInfos[i].RequireXP >= XP) return i+1;
	}
	return LevelUpInfos.Num() + 1;
}

int32 ULevelUpInfo::GetRequireXPByLevel(int32 Level)
{
	if (Level == 1) return 0;
	if (Level > 50) return 999999999;
	return LevelUpInfos[Level-2].RequireXP;
}

int32 ULevelUpInfo::GetAttributePointAwardByLevel(int32 Level)
{
	if (Level == 1) return 0;
	if (Level > 50) return 0;
	return LevelUpInfos[Level-2].AttributePointAward;
}

int32 ULevelUpInfo::GetSpellPointAwardByLevel(int32 Level)
{
	if (Level == 1) return 0;
	if (Level > 50) return 0;
	return LevelUpInfos[Level-2].SpellPointAward;
}

float ULevelUpInfo::GetLevelUpPercent(int32 XP)
{
	int32 LevelNow = FindLevelByXP(XP);
	int32 LastLevelXPRequire = GetRequireXPByLevel(LevelNow);
	return 1.f * (XP - LastLevelXPRequire) / (GetRequireXPByLevel(LevelNow + 1) - LastLevelXPRequire);
}

int32 ULevelUpInfo::GetLevelUpAttributePointReward(int32 OldLevel, int32 NewLevel)
{
	int32 AttributePointAward = 0;
	for(int i = OldLevel; i <= NewLevel; i++)
	{
		AttributePointAward += GetAttributePointAwardByLevel(i);
	}
	return AttributePointAward;
}

int32 ULevelUpInfo::GetLevelUpSpellPointReward(int32 OldLevel, int32 NewLevel)
{
	int32 SpellPointAward = 0;
	for(int i = OldLevel; i <= NewLevel; i++)
	{
		SpellPointAward += GetSpellPointAwardByLevel(i);
	}
	return SpellPointAward;
}
