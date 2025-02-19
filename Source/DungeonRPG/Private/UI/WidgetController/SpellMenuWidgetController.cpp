// DungeonRPG


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AbilityInfo.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Player/RPGPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	const ARPGPlayerState *PS = Cast<ARPGPlayerState>(PlayerState);
	OnSpellPointChanged.Broadcast(PS->GetSpellPoint());

	BroadcastAbilityInfo();
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	ARPGPlayerState *PS = Cast<ARPGPlayerState>(PlayerState);
	URPGAbilitySystemComponent *ASC = Cast<URPGAbilitySystemComponent>(AbilitySystemComponent);
	
	PS->OnSpellPointChanged.AddUObject(this, &USpellMenuWidgetController::SpellPointChanged);

	ASC->AbilityStatusChangedDelegate.AddLambda([this, ASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		if (AbilityInfo)
		{
			FRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(ASC->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = ASC->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = ASC->GetStatusTagFromSpec(AbilitySpec);
			Info.AbilityLevel = ASC->GetAbilityLevelFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
}

void USpellMenuWidgetController::SpellLevelUp(const FGameplayTag& AbilityTag)
{
	Cast<ARPGPlayerState>(PlayerState)->SpellLevelUp(AbilityTag);
}

int32 USpellMenuWidgetController::GetAbilityLevelByTag(const FGameplayTag& AbilityTag)
{
	URPGAbilitySystemComponent *ASC = Cast<URPGAbilitySystemComponent>(AbilitySystemComponent);
	if (FGameplayAbilitySpec *Spec = ASC->GetAbilitySpecFromTag(AbilityTag)) return Spec->Level;
	return -1;
}

bool USpellMenuWidgetController::GetDescription(const FGameplayTag& AbilityTag, const int32 AbilityLevel, FString& Description,
	FString& NextLevelDescription)
{
	return Cast<URPGAbilitySystemComponent>(AbilitySystemComponent)->GetDescription(AbilityTag, AbilityLevel, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpellPointChanged(const int32 SpellPoint) const
{
	OnSpellPointChanged.Broadcast(SpellPoint);
}
