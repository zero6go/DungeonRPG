// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameModeBase.generated.h"

class UAbilityInfo;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API ARPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilityInfo> AbilityInfo;

	void BeginPlay() override;
};
