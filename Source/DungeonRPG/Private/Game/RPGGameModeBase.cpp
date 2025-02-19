// DungeonRPG


#include "Game/RPGGameModeBase.h"

#include "Game/RPGGameStateBase.h"

void ARPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (ARPGGameStateBase *RPGGameState = GetGameState<ARPGGameStateBase>())
	{
		RPGGameState->AbilityInfoCopy = AbilityInfo;
	}
}
