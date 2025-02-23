// DungeonRPG


#include "AbilitySystem/Abilities/RPGBeamSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void URPGBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitActor = HitResult.GetActor();
		MouseHitLocation = HitResult.ImpactPoint;
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void URPGBeamSpell::StoreOwnerPlayerController()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
	}
}

void URPGBeamSpell::ApplyDebuff(UAbilitySystemComponent *TargetASC)
{
	const float Seed = FMath::RandRange(0.f, 100.f);
	const bool bDebuff = Seed == 100.f ? true : Seed < DebuffChance;
	if (bDebuff)
	{
		UAbilitySystemComponent *SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectSpecHandle DebuffSpecHandle = SourceASC->MakeOutgoingSpec(DebuffEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		TargetASC->ApplyGameplayEffectSpecToSelf(*DebuffSpecHandle.Data.Get());
	}
}
