// DungeonRPG


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Player/RPGPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const URPGAttributeSet *RPGAttributeSet = CastChecked<URPGAttributeSet>(AttributeSet);
	const ARPGPlayerState *RPGPlayerState = CastChecked<ARPGPlayerState>(PlayerState);
	OnHealthChanged.Broadcast(RPGAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(RPGAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(RPGAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(RPGAttributeSet->GetMaxMana());
	OnXPChanged.Broadcast(RPGPlayerState->GetPlayerXP());
	OnLevelChanged.Broadcast(RPGPlayerState->GetPlayerLevel());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	URPGAttributeSet *RPGAttributeSet = CastChecked<URPGAttributeSet>(AttributeSet);
	ARPGPlayerState *RPGPlayerState = CastChecked<ARPGPlayerState>(PlayerState);
	URPGAbilitySystemComponent *RPGASC = CastChecked<URPGAbilitySystemComponent>(AbilitySystemComponent);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetHealthAttribute()).AddUObject
	(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxHealthAttribute()).AddUObject
	(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetManaAttribute()).AddUObject
	(this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxManaAttribute()).AddUObject
	(this, &UOverlayWidgetController::MaxManaChanged);
	RPGPlayerState->OnXPChanged.AddUObject(this, &UOverlayWidgetController::XPChanged);
	RPGPlayerState->OnLevelChanged.AddUObject(this, &UOverlayWidgetController::LevelChanged);
	
	if (RPGASC)
	{
		if (RPGASC->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			RPGASC->AbilityGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
	}
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::XPChanged(const int32 NewXP) const
{
	OnXPChanged.Broadcast(NewXP);
}

void UOverlayWidgetController::LevelChanged(const int32 NewLevel) const
{
	OnLevelChanged.Broadcast(NewLevel);
}
