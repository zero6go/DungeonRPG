// DungeonRPG


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/RPGAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const URPGAttributeSet *RPGAttributeSet = CastChecked<URPGAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(RPGAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(RPGAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(RPGAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(RPGAttributeSet->GetMaxMana());
	OnStrengthChanged.Broadcast(RPGAttributeSet->GetStrength());
	OnAgilityChanged.Broadcast(RPGAttributeSet->GetAgility());
	OnIntelligenceChanged.Broadcast(RPGAttributeSet->GetIntelligence());
	OnAttackPowerChanged.Broadcast(RPGAttributeSet->GetAttackPower());
	OnArmorChanged.Broadcast(RPGAttributeSet->GetArmor());
	OnCriticalRateChanged.Broadcast(RPGAttributeSet->GetCriticalRate());
	OnCriticalDamageChanged.Broadcast(RPGAttributeSet->GetCriticalDamage());
	OnMagicPowerChanged.Broadcast(RPGAttributeSet->GetMagicPower());
	OnMagicResistanceChanged.Broadcast(RPGAttributeSet->GetMagicResistance());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	URPGAttributeSet *RPGAttributeSet = CastChecked<URPGAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetHealthAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxHealthAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetManaAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxManaAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::MaxManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetStrengthAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::StrengthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetAgilityAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::AgilityChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetIntelligenceAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::IntelligenceChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetAttackPowerAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::AttackPowerChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetArmorAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::ArmorChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetCriticalRateAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::CriticalRateChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetCriticalDamageAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::CriticalDamageChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMagicPowerAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::MagicPowerChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMagicResistanceAttribute()).AddUObject
	(this, &UAttributeMenuWidgetController::MagicResistanceChanged);
}

void UAttributeMenuWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::ArmorChanged(const FOnAttributeChangeData& Data) const
{
	OnArmorChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::AttackPowerChanged(const FOnAttributeChangeData& Data) const
{
	OnAttackPowerChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::CriticalRateChanged(const FOnAttributeChangeData& Data) const
{
	OnCriticalRateChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::CriticalDamageChanged(const FOnAttributeChangeData& Data) const
{
	OnCriticalDamageChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::MagicPowerChanged(const FOnAttributeChangeData& Data) const
{
	OnMagicPowerChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::MagicResistanceChanged(const FOnAttributeChangeData& Data) const
{
	OnMagicResistanceChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::StrengthChanged(const FOnAttributeChangeData& Data) const
{
	OnStrengthChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::AgilityChanged(const FOnAttributeChangeData& Data) const
{
	OnAgilityChanged.Broadcast(Data.NewValue);
}

void UAttributeMenuWidgetController::IntelligenceChanged(const FOnAttributeChangeData& Data) const
{
	OnIntelligenceChanged.Broadcast(Data.NewValue);
}
