// DungeonRPG


#include "Character/CharacterBase.h"

#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DungeonRPG/DungeonRPG.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BurnNiagaraComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnNiagaraComponent");
	BurnNiagaraComponent->SetupAttachment(GetRootComponent());
	BurnNiagaraComponent->DebuffTag =  FGameplayTag::RequestGameplayTag("Debuff.Burn");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, bIsStunned);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ACharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ACharacterBase::Die()
{
	if (Master) Master->MinionCount--;
	
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void ACharacterBase::MulticastHandleDeath_Implementation()
{
	GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	bDead = true;
}

int32 ACharacterBase::GetCharacterLevel()
{
	return 0;
}

void ACharacterBase::SetCharacterLevel(int32 NewLevel)
{
}

void ACharacterBase::SetMaster(ACharacterBase* NewMaster)
{
	Master = NewMaster;
}

USkeletalMeshComponent* ACharacterBase::GetWeapon() const
{
	return Weapon;
}

void ACharacterBase::Spawn()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic *DynamicInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicInstance);
		StartSpawnTimeline(DynamicInstance);
	}
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic *DynamicInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicInstance);
		StartWeaponSpawnTimeline(DynamicInstance);
	}
}

void ACharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : WalkSpeedBase;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeedBase;
	Spawn();
}

FVector ACharacterBase::GetCombatSocketLocation_Implementation(const FName &AttackSocketName)
{
	if (Weapon->GetSkeletalMeshAsset()) return Weapon->GetSocketLocation(AttackSocketName);
	else return GetMesh()->GetSocketLocation(AttackSocketName);
}

bool ACharacterBase::IsDead_Implementation() const
{
	return bDead;
}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::ApplyAttributes(TSubclassOf<UGameplayEffect> DefaultAttributes, int32 AttributeLevel)
{
	
}

void ACharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic *DynamicInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicInstance);
		StartDissolveTimeline(DynamicInstance);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic *DynamicInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicInstance);
		StartWeaponDissolveTimeline(DynamicInstance);
	}
}
