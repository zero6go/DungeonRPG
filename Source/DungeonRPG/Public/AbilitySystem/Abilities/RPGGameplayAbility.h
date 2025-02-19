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

	FGameplayTag StatusTag = FGameplayTag::RequestGameplayTag("Ability.Status.Locked");

	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	UCurveTable *DamageCurveTable;

	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	float CooldownAccordingToMMC[6];

	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	FText AbilityName;
	UPROPERTY(EditDefaultsOnly, Category = "Used by Player")
	FText AbilityDescription;

	virtual FString GetDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected:
	float GetAbilityManaCost(int32 Level) const;
};
