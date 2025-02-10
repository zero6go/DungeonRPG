// DungeonRPG


#include "Input/RPGInputConfig.h"

const UInputAction* URPGInputConfig::FindInputActionForTag(const FGameplayTag& GameplayTag) const
{
	for (auto InputAction : InputActions)
	{
		if (InputAction.InputAction && InputAction.GameplayTag == GameplayTag)
		{
			return InputAction.InputAction;
		}
	}
	return nullptr;
}
