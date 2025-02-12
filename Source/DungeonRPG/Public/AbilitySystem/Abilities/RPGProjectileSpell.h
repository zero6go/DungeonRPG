// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "RPGProjectileSpell.generated.h"

class ARPGProjectile;

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ARPGProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGProjectileSpell : public URPGGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SpawnProjectile(const FVector &TargetLocation, const FVector &WeaponTipSocketLocation, FProjectileInfo ProjectileInfo);
	
};
