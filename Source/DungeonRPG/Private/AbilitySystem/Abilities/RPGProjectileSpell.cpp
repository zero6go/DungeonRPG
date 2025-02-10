// DungeonRPG


#include "AbilitySystem/Abilities/RPGProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/RPGProjectile.h"
#include "Interaction/CombatInterface.h"

void URPGProjectileSpell::SpawnProjectile(const FVector &TargetLocation, const FVector &WeaponTipSocketLocation)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	ICombatInterface *CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface) return;

	check(ProjectileClass);
	FRotator Rotation = (TargetLocation - WeaponTipSocketLocation).Rotation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(WeaponTipSocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	ARPGProjectile *Projectile =  GetWorld()->SpawnActorDeferred<ARPGProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	UAbilitySystemComponent *SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
	Projectile->DamageEffectSpecHandle = SpecHandle;

	Projectile->FinishSpawning(SpawnTransform);
}
