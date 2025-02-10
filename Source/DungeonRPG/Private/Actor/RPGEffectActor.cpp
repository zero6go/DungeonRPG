// DungeonRPG


#include "Actor/RPGEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

ARPGEffectActor::ARPGEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
}



void ARPGEffectActor::BeginPlay()
{
	Super::BeginPlay();

	
}

void ARPGEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	if (IAbilitySystemInterface *ASI = Cast<IAbilitySystemInterface>(TargetActor))
	{
		UAbilitySystemComponent *ASC = ASI->GetAbilitySystemComponent();
		if (ASC == nullptr) return;

		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bIsInfinite)
		{
			GameplayEffectHash.Add(ActiveGameplayEffectHandle, ASC);
		}
	}
}

void ARPGEffectActor::RemoveEffectFromTarget(AActor* TargetActor)
{
	if (IAbilitySystemInterface *ASI = Cast<IAbilitySystemInterface>(TargetActor))
	{
		UAbilitySystemComponent *ASC = ASI->GetAbilitySystemComponent();
		if (ASC == nullptr) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for(auto Pair:GameplayEffectHash)
		{
			if (Pair.Value == ASC)
			{
				ASC->RemoveActiveGameplayEffect(Pair.Key, 1);
				HandlesToRemove.Add(Pair.Key);
			}
		}
		for (auto ActiveGameplayEffect : HandlesToRemove)
		{
			GameplayEffectHash.FindAndRemoveChecked(ActiveGameplayEffect);
		}
	}
}


