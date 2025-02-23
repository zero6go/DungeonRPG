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
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float XPRewardRate = 1.f;

	virtual void Die() override;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

protected:
	virtual void BeginPlay() override;;
	virtual void InitAbilityActorInfo() override;
	virtual void ApplyAttributes(TSubclassOf<UGameplayEffect> DefaultAttributes, int32 AttributeLevel) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ARPGAIController> RPGAIController;
};
