// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RPGInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FRPGInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction *InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameplayTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URPGInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction *FindInputActionForTag(const FGameplayTag &GameplayTag) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRPGInputAction> InputActions;
};
