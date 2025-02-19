// DungeonRPG


#include "Game/RPGGameStateBase.h"
#include "AbilitySystem/AbilityInfo.h"
#include "Net/UnrealNetwork.h"

void ARPGGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARPGGameStateBase, AbilityInfoCopy);
}
