// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "WaitCooldownChange.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemain);

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class DUNGEONRPG_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange *WaitCooldownChange(UAbilitySystemComponent *AbilitySystemComponent, const FGameplayTag &CooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent *ASC;

	UPROPERTY()
	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent *TargetASC, const FGameplayEffectSpec &Spec, FActiveGameplayEffectHandle ActiveEffectHandle);
};