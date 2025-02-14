// DungeonRPG


#include "AbilitySystem/Abilities/RPGSummonMinion.h"

#include "NavigationSystem.h"
#include "Character/CharacterBase.h"
#include "Player/RPGPlayerState.h"

void URPGSummonMinion::SpawnMinion(TSubclassOf<ACharacterBase> MinionClass, const FVector &SpawnLocation, int32 MinionLevel)
{
	FRotator Rotation = GetAvatarActorFromActorInfo()->GetActorRotation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	ACharacterBase *Minion = GetWorld()->SpawnActorDeferred<ACharacterBase>(MinionClass, SpawnTransform, GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Minion->SetCharacterLevel(MinionLevel);
	Minion->SpawnDefaultController();
	if (ARPGPlayerState *PS = Cast<ARPGPlayerState>(GetAvatarActorFromActorInfo())) Minion->SetMaster(Cast<ACharacterBase>(PS->GetPawn()));
	else Minion->SetMaster(Cast<ACharacterBase>(GetAvatarActorFromActorInfo()));
	Minion->FinishSpawning(SpawnTransform);
}

FVector URPGSummonMinion::GetSpawnLocation()
{
	FVector Origin = GetAvatarActorFromActorInfo()->GetActorLocation();
	float Radius = 500.0f;
	FVector RandomLocation = Origin + GetAvatarActorFromActorInfo()->GetActorForwardVector();
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem)
	{
		NavSystem->K2_GetRandomLocationInNavigableRadius(GetWorld(), Origin, RandomLocation, Radius);
	}
	RandomLocation.Z += 50.f;
	return RandomLocation;
}

TSubclassOf<ACharacterBase> URPGSummonMinion::GetRandomMinionClass()
{
	int32 idx = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[idx];
}
