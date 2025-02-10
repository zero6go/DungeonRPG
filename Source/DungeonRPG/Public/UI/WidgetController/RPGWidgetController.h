// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPGWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController *PC, APlayerState *PS, UAbilitySystemComponent *ASC, UAttributeSet *AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {} 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackPowerChangedSignature, float, NewAttackPower);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArmorChangedSignature, float, NewArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCriticalRateChangedSignature, float, NewCriticalRate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCriticalDamageChangedSignature, float, NewCriticalDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMagicPowerChangedSignature, float, NeMagicPower);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMagicResistanceChangedSignature, float, NewMagicResistance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrengthChangedSignature, float, NewStrength);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAgilityChangedSignature, float, NewAgility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntelligenceChangedSignature, float, NewIntelligence);

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams &WidgetControllerParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
