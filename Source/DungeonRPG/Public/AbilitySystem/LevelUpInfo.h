// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FRPGLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 RequireXP;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward;
};

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FRPGLevelUpInfo> LevelUpInfos;

	UFUNCTION(BlueprintCallable)
	int32 FindLevelByXP(int32 XP);
	
	int32 GetRequireXPByLevel(int32 Level);
	int32 GetAttributePointAwardByLevel(int32 Level);
	int32 GetSpellPointAwardByLevel(int32 Level);

	UFUNCTION(BlueprintCallable)
	float GetLevelUpPercent(int32 XP);

	int32 GetLevelUpAttributePointReward(int32 OldLevel, int32 NewLevel);
	int32 GetLevelUpSpellPointReward(int32 OldLevel, int32 NewLevel);
};
