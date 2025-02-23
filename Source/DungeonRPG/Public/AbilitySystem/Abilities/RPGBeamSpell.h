// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "RPGBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGBeamSpell : public URPGGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerPlayerController();

	UFUNCTION(BlueprintCallable)
	void ApplyDebuff(UAbilitySystemComponent *TargetASC);
protected:

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
};
