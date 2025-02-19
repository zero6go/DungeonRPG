// DungeonRPG


#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "CharacterBase.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS(BlueprintType)
class DUNGEONRPG_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	virtual UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual void Die() override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<FAttackInfo> AttackInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<FRangedAttackInfo> RangedAttackInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<FAbilityClassAndLevel> StartupAbilities;

	UFUNCTION(BlueprintPure)
	virtual int32 GetCharacterLevel();

	UFUNCTION(BlueprintCallable)
	virtual void SetCharacterLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable)
	void SetMaster(ACharacterBase *NewMaster);

	USkeletalMeshComponent* GetWeapon() const;

	void Spawn();
	void Dissolve();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float WalkSpeedBase = 250.f;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	virtual FVector GetCombatSocketLocation_Implementation(const FName &AttackSocketName) override;

	bool bDead = false;
	virtual bool IsDead_Implementation() const override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	virtual void ApplyAttributes(TSubclassOf<UGameplayEffect> DefaultAttributes, int32 AttributeLevel);

	UFUNCTION(BlueprintImplementableEvent)
	void StartSpawnTimeline(UMaterialInstanceDynamic *DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponSpawnTimeline(UMaterialInstanceDynamic *DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic *DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic *DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMaterialInterface> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMaterialInterface> WeaponDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem *BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase *DeathSound;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	int32 MinionCount = 0;

	TObjectPtr<ACharacterBase> Master = nullptr;

private:

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* HitReactMontage;
};
