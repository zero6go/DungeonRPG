// DungeonRPG


#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"

void URPGAbilitySystemComponent::GivePlayerAbility(const FAbilityClassAndLevel& AbilityInfo)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.AbilityClass, AbilityInfo.Level);
	if (URPGGameplayAbility *RPGAbility = Cast<URPGGameplayAbility>(AbilitySpec.Ability))
	{
		AbilitySpec.DynamicAbilityTags.AddTag(RPGAbility->InputTag);
		GiveAbility(AbilitySpec);
		NetMulticast_AbilityGiven(RPGAbility->AbilityTag, RPGAbility->InputTag);
	}
}

void URPGAbilitySystemComponent::GiveCharacterAbility(const FAbilityClassAndLevel& AbilityInfo)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.AbilityClass, AbilityInfo.Level);
	GiveAbility(AbilitySpec);
}

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

void URPGAbilitySystemComponent::NetMulticast_AbilityGiven_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& InputTag)
{
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilityGivenDelegate.Broadcast(AbilityTag, InputTag);
	}
}
