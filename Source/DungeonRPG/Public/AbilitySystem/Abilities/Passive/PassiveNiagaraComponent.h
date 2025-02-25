// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "PassiveNiagaraComponent.generated.h"


/**
 * 
 */
UCLASS()
class DUNGEONRPG_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UPassiveNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag BuffTag;

protected:
	virtual void BeginPlay() override;

	void BuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
