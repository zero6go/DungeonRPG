// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "RPGProjectileSpell.generated.h"

class ARPGProjectile;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGProjectileSpell : public URPGGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SpawnProjectile(const FVector &TargetLocation, const FVector &WeaponTipSocketLocation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARPGProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
