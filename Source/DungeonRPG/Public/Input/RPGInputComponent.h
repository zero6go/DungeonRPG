// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "RPGInputConfig.h"
#include "RPGInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const URPGInputConfig *RPGInputConfig, UserClass *Object, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc);
};

template <class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
void URPGInputComponent::BindAbilityActions(const URPGInputConfig* RPGInputConfig, UserClass* Object,
	PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
{
	check(RPGInputConfig);

	for (auto InputAction : RPGInputConfig->InputActions)
	{
		if (InputAction.InputAction && InputAction.GameplayTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputAction.GameplayTag);
			}
			if (HeldFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputAction.GameplayTag);
			}
			if (ReleasedFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.GameplayTag);
			}
		}
	}
}
