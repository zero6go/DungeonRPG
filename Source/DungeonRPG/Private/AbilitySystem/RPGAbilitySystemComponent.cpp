// DungeonRPG


#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"

void URPGAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	
}

void URPGAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void URPGAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}
