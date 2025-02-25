// DungeonRPG


#include "AbilitySystem/RPGAbilitySystemComponent.h"

#include "AbilitySystem/AbilityInfo.h"
#include "AbilitySystem/RPGAbilitySystemFunctionLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "UI/WidgetController/OverlayWidgetController.h"

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
	if (!InputTag.IsValid()) return;

	if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag.RMB"))) return;
	else if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag.LMB")))
	{
		FGameplayAbilitySpec *EquippedAbility = GetAbilitySpecFromAbilityTag(EquippedAbilityTag);
		if (EquippedAbility && EquippedAbility->Ability)
		{
			AbilitySpecInputPressed(*EquippedAbility);
			if(EquippedAbility->IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, EquippedAbility->Handle,
					EquippedAbility->ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
	else
	{
		for (auto AbilitySpec : GetActivatableAbilities())
		{
			if(AbilitySpec.Ability)
			{
				URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec.Ability);
				FGameplayAbilitySpec *EquippedAbility = GetAbilitySpecFromAbilityTag(EquippedAbilityTag);
				if (Ability->InputTag.MatchesTag(InputTag) && (!EquippedAbility || !EquippedAbility->IsActive()) && !HasMatchingGameplayTag(Ability->CooldownTag))
				{
					EquippedAbilityTag = Ability->AbilityTag;
					const UAbilityInfo *AbilityInfo = URPGAbilitySystemFunctionLibrary::GetAbilityInfo(GetAvatarActor());
					URPGAbilitySystemFunctionLibrary::GetOverlayWidgetController(GetAvatarActor())
					->OnSpellEquipped.Broadcast(AbilityInfo->FindAbilityInfoForTag(Ability->AbilityTag));
				}
			}
		}
	}
}

void URPGAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag.LMB")))
	{
		FGameplayAbilitySpec *EquippedAbility = GetAbilitySpecFromAbilityTag(EquippedAbilityTag);
		if (EquippedAbility && EquippedAbility->Ability)
		{
			AbilitySpecInputPressed(*EquippedAbility);
			if(!EquippedAbility->IsActive())
			{
				TryActivateAbility(EquippedAbility->Handle);
			}
		}
	}
	if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag.RMB")))
	{
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
}

void URPGAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag.LMB")))
	{
		FGameplayAbilitySpec *EquippedAbility = GetAbilitySpecFromAbilityTag(EquippedAbilityTag);
		if (EquippedAbility && EquippedAbility->Ability && EquippedAbility->IsActive())
		{
			AbilitySpecInputReleased(*EquippedAbility);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, EquippedAbility->Handle,
					EquippedAbility->ActivationInfo.GetActivationPredictionKey());
		}
	}
	if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag.RMB")))
	{
		for (auto AbilitySpec : GetActivatableAbilities())
		{
			if(AbilitySpec.Ability)
			{
				URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec.Ability);
				if (Ability->InputTag.MatchesTag(InputTag) && AbilitySpec.IsActive())
				{
					AbilitySpecInputReleased(AbilitySpec);
				}
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

FGameplayAbilitySpec* URPGAbilitySystemComponent::GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag)
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

FGameplayAbilitySpec* URPGAbilitySystemComponent::GetAbilitySpecFromInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability)
		{
			URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec.Ability);
			if (Ability->InputTag.MatchesTag(InputTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void URPGAbilitySystemComponent::EquipSpell(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec *Spec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		//卸下装备槽的原技能
		if (FGameplayAbilitySpec *OldSpec = GetAbilitySpecFromInputTag(InputTag))
		{
			URPGGameplayAbility *OldAbility = Cast<URPGGameplayAbility>(OldSpec->Ability);
			OldAbility->InputTag = FGameplayTag();
			OldAbility->StatusTag = FGameplayTag::RequestGameplayTag("Ability.Status.UnLocked");
			if (OldAbility->IsPassiveAbility) DeactivatePassiveAbilityDelegate.Broadcast(OldAbility->AbilityTag);
		}
		URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(Spec->Ability);
		//新技能若已装备在别的槽位则卸下
		if (Ability->StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag("Ability.Status.Equipped")))
		{
			Ability->InputTag = FGameplayTag();
			Ability->StatusTag = FGameplayTag::RequestGameplayTag("Ability.Status.UnLocked");
			if (Ability->IsPassiveAbility) DeactivatePassiveAbilityDelegate.Broadcast(AbilityTag);
		}
		Ability->InputTag = InputTag;
		Ability->StatusTag = FGameplayTag::RequestGameplayTag("Ability.Status.Equipped");
		if (Ability->IsPassiveAbility) TryActivateAbility(Spec->Handle);
	}
}

void URPGAbilitySystemComponent::UpdateAbilityStatusUnlockable(int32 OldLevel, int32 NewLevel)
{
	const UAbilityInfo *AbilityInfo = URPGAbilitySystemFunctionLibrary::GetAbilityInfo(GetAvatarActor());
	for (auto Info : AbilityInfo->AbilityInformation)
	{
		if (NewLevel >= Info.LevelRequirement && OldLevel < Info.LevelRequirement)
		{
			if (!GetAbilitySpecFromAbilityTag(Info.AbilityTag))
			{
				FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Info.AbilityClass, 0);
				URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(Spec.Ability);
				Ability->StatusTag = FGameplayTag::RequestGameplayTag("Ability.Status.Unlockable");
				Ability->InputTag = FGameplayTag();
				GiveAbility(Spec);
				MarkAbilitySpecDirty(Spec);
				ClientUpdateAbilityStatus(Spec);
			}
		}
		
	}
}

void URPGAbilitySystemComponent::ServerSpellLevelUp_Implementation(const FGameplayTag& AbilityTag)
{
	const UAbilityInfo *AbilityInfo = URPGAbilitySystemFunctionLibrary::GetAbilityInfo(GetAvatarActor());
	for (auto Info : AbilityInfo->AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTag(AbilityTag))
		{
			FGameplayAbilitySpec *Spec = GetAbilitySpecFromAbilityTag(Info.AbilityTag);
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

bool URPGAbilitySystemComponent::GetDescription(const FGameplayTag& AbilityTag, const int32 AbilityLevel, FString& Description,
	FString& NextLevelDescription)
{
	if (const FGameplayAbilitySpec *AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		if (URPGGameplayAbility *Ability = Cast<URPGGameplayAbility>(AbilitySpec->Ability))
		{
			if (AbilityLevel > 0)
			{
				Description = Ability->GetDescription(AbilityLevel);
				NextLevelDescription = Ability->GetDescription(AbilityLevel + 1);
				return true;
			}
			else if (AbilityLevel == 0)
			{
				Description = FString("<Default>技能可以花费一技能点解锁</>");
				NextLevelDescription = Ability->GetDescription(AbilityLevel + 1);
			}
			
		}
	}
	const UAbilityInfo *AbilityInfo = URPGAbilitySystemFunctionLibrary::GetAbilityInfo(GetAvatarActor());
	Description = URPGGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	NextLevelDescription = FString();
	return false;
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
