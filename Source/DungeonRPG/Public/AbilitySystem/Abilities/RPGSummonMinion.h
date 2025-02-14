// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "RPGSummonMinion.generated.h"

class UNiagaraSystem;
class ACharacterBase;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGSummonMinion : public URPGGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Minion")
	void SpawnMinion(TSubclassOf<ACharacterBase> MinionClass, const FVector &SpawnLocation, int32 MinionLevel);

	UFUNCTION(BlueprintCallable, Category = "Minion")
	FVector GetSpawnLocation();

	UFUNCTION(BlueprintPure, Category = "Minion")
	TSubclassOf<ACharacterBase> GetRandomMinionClass();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem *SummonEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Minion")
	TArray<TSubclassOf<ACharacterBase>> MinionClasses;
};
