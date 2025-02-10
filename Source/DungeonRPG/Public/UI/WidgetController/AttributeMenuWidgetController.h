// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/RPGWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FOnAttributeChangeData;


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DUNGEONRPG_API UAttributeMenuWidgetController : public URPGWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttackPowerChangedSignature OnAttackPowerChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnArmorChangedSignature OnArmorChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnCriticalRateChangedSignature OnCriticalRateChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnCriticalDamageChangedSignature OnCriticalDamageChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMagicPowerChangedSignature OnMagicPowerChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMagicResistanceChangedSignature OnMagicResistanceChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnStrengthChangedSignature OnStrengthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAgilityChangedSignature OnAgilityChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnIntelligenceChangedSignature OnIntelligenceChanged;

protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	void ArmorChanged(const FOnAttributeChangeData& Data) const;
	void AttackPowerChanged(const FOnAttributeChangeData& Data) const;
	void CriticalRateChanged(const FOnAttributeChangeData& Data) const;
	void CriticalDamageChanged(const FOnAttributeChangeData& Data) const;
	void MagicPowerChanged(const FOnAttributeChangeData& Data) const;
	void MagicResistanceChanged(const FOnAttributeChangeData& Data) const;
	void StrengthChanged(const FOnAttributeChangeData& Data) const;
	void AgilityChanged(const FOnAttributeChangeData& Data) const;
	void IntelligenceChanged(const FOnAttributeChangeData& Data) const;
};
