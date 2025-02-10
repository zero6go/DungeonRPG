// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "RPGPlayerController.generated.h"

class UDamageTextComponent;
class URPGInputConfig;
class URPGAbilitySystemComponent;
struct FGameplayTag;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ARPGPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageValue(float Damage, ACharacter* TargetCharacter, bool bCriticalHit);
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	FVector ForwardVector;

	TObjectPtr<URPGAbilitySystemComponent> AbilitySystemComponent;
	URPGAbilitySystemComponent *GetAbilitySystemComponent();

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<URPGInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
