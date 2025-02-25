// DungeonRPG


#include "AbilitySystem/Abilities/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent *ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(BuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UPassiveNiagaraComponent::BuffTagChanged);
	}
	else if (APlayerCharacter *Player = Cast<APlayerCharacter>(GetOwner()))
	{
		Player->OnASCRegisteredDelegate.AddLambda([this](UAbilitySystemComponent *ASC)
		{
			ASC->RegisterGameplayTagEvent(BuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UPassiveNiagaraComponent::BuffTagChanged);
		});
	}
}

void UPassiveNiagaraComponent::BuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0) Activate();
	else Deactivate();
}
