// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerCharacter.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	APlayerCharacter();

	void PossessedBy(AController* NewController) override;
	void OnRep_PlayerState() override;

	UFUNCTION(Server, Reliable)
	void GivePlayerStartupAbilities();

	virtual int32 GetCharacterLevel() override;
	virtual void SetCharacterLevel(int32 NewLevel) override;

	virtual UAttributeSet* GetAttributeSet() const override;

	virtual void Die() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> RunNiagaraComponent;
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastLevelUp();

	UFUNCTION(BlueprintCallable)
	void Run();

	void Walk();
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> StrengthPointEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> AgilityPointEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> IntelligencePointEffect;

	UFUNCTION(Server, Reliable)
	void UpgradeAttribute(const FName &AttributeTag, const int32 Point);

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void ApplyAttributes(TSubclassOf<UGameplayEffect> DefaultAttributes, int32 AttributeLevel) override;
	void ApplyPlayerAttributes();
};
