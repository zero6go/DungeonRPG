// DungeonRPG


#include "UI/WidgetController/RPGWidgetController.h"
#include "AbilitySystem/AbilityInfo.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"

void URPGWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void URPGWidgetController::BroadcastInitialValues()
{
}

void URPGWidgetController::BindCallbacksToDependencies()
{
}

void URPGWidgetController::BroadcastAbilityInfo()
{
	URPGAbilitySystemComponent *ASC = Cast<URPGAbilitySystemComponent>(AbilitySystemComponent);
	if (!ASC->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([ASC, this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(ASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = ASC->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = ASC->GetStatusTagFromSpec(AbilitySpec);
		Info.AbilityLevel = ASC->GetAbilityLevelFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	ASC->ForEachAbility(BroadcastDelegate);
}