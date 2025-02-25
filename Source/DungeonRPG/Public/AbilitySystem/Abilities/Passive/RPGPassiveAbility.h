// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "RPGPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGPassiveAbility : public URPGGameplayAbility
{
	GENERATED_BODY()

public:
	URPGPassiveAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag BuffTag;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ReceiveDeactivate(const FGameplayTag &InAbilityTag);

	UFUNCTION(BlueprintCallable)
	void ApplyEffect(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level = 1.f);

	UFUNCTION(BlueprintCallable)
	void RemoveEffect();

	UPROPERTY(EditDefaultsOnly)
	UCurveTable *RateCurveTable;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	virtual FString GetDescription(int32 Level) override;
};
