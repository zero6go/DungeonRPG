// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage *Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttackStartTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttackEndTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttackSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackRadius;
};

USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	float Level = 1.f;
};

class DUNGEONRPG_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetCombatSocketLocation(const FName &AttackSocketName);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFacingTargetLocation(const FVector &Location);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;
};
