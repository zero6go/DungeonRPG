// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "RestoreTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRPG_API URestoreTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetRestoreText(float Restore, bool bCriticalHit, bool bHeal);
};
