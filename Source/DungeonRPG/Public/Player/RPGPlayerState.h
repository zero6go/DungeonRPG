// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "RPGPlayerState.generated.h"

/**
 * 
 */

struct FGameplayTag;
class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, int32);

UCLASS()
class DUNGEONRPG_API ARPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ARPGPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	void SetPlayerLevel(int32 NewLevel);
	void LevelUp(int32 OldLevel, int32 NewLevel);
	FOnPlayerStateChanged OnLevelChanged;

	FORCEINLINE int32 GetPlayerXP() const { return XP; }
	void SetPlayerXP(int32 NewXP);
	FOnPlayerStateChanged OnXPChanged;
	void AddXP(float XPRewardRate, int32 EnemyLV);

	FOnPlayerStateChanged OnAttributePointChanged;
	FORCEINLINE int32 GetAttributePoint() const { return AttributePoint; }
	void AddAttributePoint(int32 InAttributePoint);
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FName &AttributeTag);
	
	FOnPlayerStateChanged OnSpellPointChanged;
	FORCEINLINE int32 GetSpellPoint() const { return SpellPoint; }
	void AddSpellPoint(int32 InSpellPoint);
	UFUNCTION(Server, Reliable)
	void ServerSpellLevelUp(const FGameplayTag &AbilityTag);

	FORCEINLINE int32 GetStrengthPoint() const { return StrengthPoint; }
	FORCEINLINE int32 GetAgilityPoint() const { return AgilityPoint; }
	FORCEINLINE int32 GetIntelligencePoint() const { return IntelligencePoint; }
	void AddStrengthPoint(int32 InStrengthPoint);
	void AddAgilityPoint(int32 InAgilityPoint);
	void AddIntelligencePoint(int32 InIntelligencePoint);
	
	void Init();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	int32 NextLevelXPRequire;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoint)
	int32 AttributePoint = 0;
	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoint)
	int32 SpellPoint = 0;
	UFUNCTION()
	void OnRep_SpellPoint(int32 OldSpellPoint);

	UPROPERTY(VisibleAnywhere, Replicated)
	int32 StrengthPoint = 0;
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 AgilityPoint = 0;
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 IntelligencePoint = 0;
};
