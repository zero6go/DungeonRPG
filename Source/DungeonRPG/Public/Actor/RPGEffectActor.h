// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "RPGEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class USphereComponent;

UCLASS()
class DUNGEONRPG_API ARPGEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPGEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor *TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level = 1.f);

	UFUNCTION(BlueprintCallable)
	void RemoveEffectFromTarget(AActor *TargetActor);
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> GameplayEffectHash;
};
