// DungeonRPG


#include "Actor/RPGProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "DungeonRPG/DungeonRPG.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RPGPlayerState.h"

ARPGProjectile::ARPGProjectile()
{
 	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ARPGProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);

	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ARPGProjectile::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void ARPGProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const ARPGPlayerState *PS = Cast<ARPGPlayerState>(GetOwner()))
	{
		if (PS->GetPawn() == OtherActor) return;
	}
	if (GetOwner()->ActorHasTag("Enemy") && OtherActor->ActorHasTag("Enemy")) return;
	if (GetOwner() != OtherActor)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();

		if (HasAuthority())
		{
			if (UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
				if (DamageEffectSpecHandle.Data.Get() != nullptr)
				{
					const float Seed = FMath::RandRange(0.f, 100.f);
					const bool bDebuff = Seed == 100.f ? true : Seed < DebuffChance;
					if (bDebuff)
					{
						TargetASC->ApplyGameplayEffectSpecToSelf(*DebuffEffectSpecHandle.Data.Get());
					}
				}
			}
		
			Destroy();
		}
	}
}

