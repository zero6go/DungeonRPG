// DungeonRPG


#include "AbilitySystem/Abilities/RPGProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/RPGProjectile.h"
#include "Interaction/CombatInterface.h"

void URPGProjectileSpell::SpawnProjectile(const FVector &TargetLocation, const FVector &WeaponTipSocketLocation, FProjectileInfo ProjectileInfo)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	ICombatInterface *CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface) return;
	
	FRotator Rotation = (TargetLocation - WeaponTipSocketLocation).Rotation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(WeaponTipSocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	ARPGProjectile *Projectile =  GetWorld()->SpawnActorDeferred<ARPGProjectile>(ProjectileInfo.ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	UAbilitySystemComponent *SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(ProjectileInfo.DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
	Projectile->DamageEffectSpecHandle = SpecHandle;
	if (DebuffEffectClass)
	{
		Projectile->DebuffChance = this->DebuffChance;
		FGameplayEffectSpecHandle DebuffSpecHandle = SourceASC->MakeOutgoingSpec(DebuffEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		Projectile->DebuffEffectSpecHandle = DebuffSpecHandle;
	}

	Projectile->FinishSpawning(SpawnTransform);
}
