// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RPGGameStateBase.generated.h"

class UAbilityInfo;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API ARPGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	const UAbilityInfo *AbilityInfoCopy;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
