// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerCharacter.generated.h"

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

protected:
	virtual void InitAbilityActorInfo() override;

	virtual int32 GetCharacterLevel() override;
};
