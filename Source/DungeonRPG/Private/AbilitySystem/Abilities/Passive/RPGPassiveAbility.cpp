// DungeonRPG


#include "AbilitySystem/Abilities/Passive/RPGPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"

URPGPassiveAbility::URPGPassiveAbility()
{
	IsPassiveAbility = true;
}

void URPGPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (URPGAbilitySystemComponent *ASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		ASC->DeactivatePassiveAbilityDelegate.AddUObject(this, &URPGPassiveAbility::ReceiveDeactivate);
	}
}

void URPGPassiveAbility::ReceiveDeactivate(const FGameplayTag& InAbilityTag)
{
	if (InAbilityTag == this->AbilityTag)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void URPGPassiveAbility::ApplyEffect(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	if (IAbilitySystemInterface *ASI = Cast<IAbilitySystemInterface>(GetAvatarActorFromActorInfo()))
	{
		UAbilitySystemComponent *ASC = ASI->GetAbilitySystemComponent();
		if (ASC == nullptr) return;

		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void URPGPassiveAbility::RemoveEffect()
{
	if (IAbilitySystemInterface *ASI = Cast<IAbilitySystemInterface>(GetAvatarActorFromActorInfo()))
	{
		UAbilitySystemComponent *ASC = ASI->GetAbilitySystemComponent();
		if (ASC == nullptr) return;

		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(BuffTag);
		ASC->RemoveActiveEffectsWithGrantedTags(TagContainer);
	}
}

FString URPGPassiveAbility::GetDescription(int32 Level)
{
	if (Level > 6) return FString::Printf(TEXT("<Default>已满级</>"));

	float Rate = 0.f;
	if (RateCurveTable) Rate = RateCurveTable->FindCurve("Rate", "")->Eval(Level);
	
	FString Description = FString::Printf(TEXT("<Title>%s</>\n\n<Default>%s</>\n\n"), *AbilityName.ToString(), *AbilityDescription.ToString());
	FString LevelDescription = FString::Printf(TEXT("<Default>等级： </><Level>%d</>\n"), Level);
	FString RateDescription = FString::Printf(TEXT("<Default>比例： </><Percent>%.2f%%</>\n"), Rate);
	
	return Description + LevelDescription + RateDescription;
}
