// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RPGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	FGameplayTag CooldownTag;

	bool IsPassiveAbility = false;

	FGameplayTag StatusTag = FGameplayTag::RequestGameplayTag("Ability.Status.Locked");

	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	UCurveTable *DamageCurveTable;

	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	float CooldownAccordingToMMC[6];

	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	FText AbilityName;
	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	FText AbilityDescription;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffChance = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	TSubclassOf<UGameplayEffect> DebuffEffectClass;

	virtual FString GetDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected:
	float GetAbilityManaCost(int32 Level) const;
};
