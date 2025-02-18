// DungeonRPG


#include "AbilitySystem/RPGAbilitySystemComponent.h"

#include "AbilitySystem/AbilityInfo.h"
#include "AbilitySystem/RPGAbilitySystemFunctionLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"

void URPGAbilitySystemComponent::GivePlayerAbility(const FAbilityClassAndLevel& AbilityInfo)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.AbilityClass, AbilityInfo.Level);
	Cast<URPGGameplayAbility>(AbilitySpec.Ability)->StatusTag = FGameplayTag::RequestGameplayTag("Ability.Status.Equipped");
	GiveAbility(AbilitySpec);
	bStartupAbilitiesGiven = true;
	AbilityGivenDelegate.Broadcast();
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
		if(AbilitySpec.Ability)
		{
			URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec.Ability);
			if (Ability->InputTag.MatchesTag(InputTag))
			{
				AbilitySpecInputPressed(AbilitySpec);
				if(!AbilitySpec.IsActive())
				{
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
		}
	}
}

void URPGAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability)
		{
			URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec.Ability);
			if (Ability->InputTag.MatchesTag(InputTag))
			{
				AbilitySpecInputReleased(AbilitySpec);
			}
		}
	}
}

void URPGAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			
		}
	}
}

FGameplayTag URPGAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec.Ability);
		return Ability->AbilityTag;
	}
	return FGameplayTag();
}

FGameplayTag URPGAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec.Ability);
		return Ability->InputTag;
	}
	return FGameplayTag();
}

FGameplayTag URPGAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec.Ability);
		return Ability->StatusTag;
	}
	return FGameplayTag();
}

int32 URPGAbilitySystemComponent::GetAbilityLevelFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	return AbilitySpec.Level;
}

FGameplayAbilitySpec* URPGAbilitySystemComponent::GetAbilitySpecFromTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability)
		{
			URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec.Ability);
			if (Ability->AbilityTag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void URPGAbilitySystemComponent::UpdateAbilityStatusUnlockable(int32 OldLevel, int32 NewLevel)
{
	UAbilityInfo *AbilityInfo = URPGAbilitySystemFunctionLibrary::GetAbilityInfo(GetAvatarActor());
	for (auto Info : AbilityInfo->AbilityInformation)
	{
		if (NewLevel >= Info.LevelRequirement && OldLevel < Info.LevelRequirement)
		{
			if (!GetAbilitySpecFromTag(Info.AbilityTag))
			{
				FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Info.AbilityClass, 0);
				Cast<URPGGameplayAbility>(Spec.Ability)->StatusTag = FGameplayTag::RequestGameplayTag("Ability.Status.Unlockable");
				GiveAbility(Spec);
				MarkAbilitySpecDirty(Spec);
				ClientUpdateAbilityStatus(Spec);
			}
		}
		
	}
}

void URPGAbilitySystemComponent::ServerSpellLevelUp_Implementation(const FGameplayTag& AbilityTag)
{
	UAbilityInfo *AbilityInfo = URPGAbilitySystemFunctionLibrary::GetAbilityInfo(GetAvatarActor());
	for (auto Info : AbilityInfo->AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTag(AbilityTag))
		{
			FGameplayAbilitySpec *Spec = GetAbilitySpecFromTag(Info.AbilityTag);
			if (Spec->Level == 0)
			{
				Cast<URPGGameplayAbility>(Spec->Ability)->StatusTag = FGameplayTag::RequestGameplayTag("Ability.Status.Unlocked");
			}
			Spec->Level++;
			MarkAbilitySpecDirty(*Spec);
			ClientUpdateAbilityStatus(*Spec);
		}
	}
}

void URPGAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilityGivenDelegate.Broadcast();
	}
}

void URPGAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayAbilitySpec& AbilitySpec)
{
	AbilityStatusChangedDelegate.Broadcast(AbilitySpec);
}
