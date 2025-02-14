// DungeonRPG


#include "AbilitySystem/AbilityInfo.h"

FRPGAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag) const
{
	for (const FRPGAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	return FRPGAbilityInfo();
}
