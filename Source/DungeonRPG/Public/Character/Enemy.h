// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Enemy.generated.h"

class ARPGAIController;
class UBehaviorTree;
struct FGameplayTag;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API AEnemy : public ACharacterBase
{
	GENERATED_BODY()
public:
	AEnemy();
	
	virtual int32 GetCharacterLevel() override;
	virtual void SetCharacterLevel(int32 NewLevel) override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float WalkSpeedBase = 250.f;

	virtual void Die() override;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ARPGAIController> RPGAIController;
};
