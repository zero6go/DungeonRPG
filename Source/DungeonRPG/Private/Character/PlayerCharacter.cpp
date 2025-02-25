// DungeonRPG


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "DungeonRPG/DungeonRPG.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/RPGPlayerController.h"
#include "Player/RPGPlayerState.h"
#include "UI/HUD/RPGHUD.h"
#include "AbilitySystem/Abilities/Passive/PassiveNiagaraComponent.h"

APlayerCharacter::APlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(RootComponent);
	LevelUpNiagaraComponent->SetAutoActivate(false);

	RunNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("RunNiagaraComponent");
	RunNiagaraComponent->SetupAttachment(RootComponent);
	RunNiagaraComponent->SetAutoActivate(false);

	HaloNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloNiagaraComponent");
	HaloNiagaraComponent->SetupAttachment(GetRootComponent());
	HaloNiagaraComponent->BuffTag =  FGameplayTag::RequestGameplayTag("Buff.HaloOfProtection");

	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonNiagaraComponent");
	LifeSiphonNiagaraComponent->SetupAttachment(GetRootComponent());
	LifeSiphonNiagaraComponent->BuffTag =  FGameplayTag::RequestGameplayTag("Buff.LifeSiphon");

	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphonNiagaraComponent");
	ManaSiphonNiagaraComponent->SetupAttachment(GetRootComponent());
	ManaSiphonNiagaraComponent->BuffTag =  FGameplayTag::RequestGameplayTag("Buff.ManaSiphon");
}

//服务器
void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	ApplyPlayerAttributes();
	if (IsLocallyControlled()) GivePlayerStartupAbilities();
}

//客户端
void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
	GivePlayerStartupAbilities();
}

void APlayerCharacter::GivePlayerStartupAbilities_Implementation()
{
	URPGAbilitySystemComponent *ASC = Cast<URPGAbilitySystemComponent>(AbilitySystemComponent);
	for(auto Ability : StartupAbilities)
	{
		ASC->GivePlayerAbility(Ability);
	}
}

void APlayerCharacter::NetMulticastLevelUp_Implementation()
{
	if (IsValid(LevelUpNiagaraComponent)) LevelUpNiagaraComponent->Activate(true);
	ApplyPlayerAttributes();
}

void APlayerCharacter::UpgradeAttribute_Implementation(const FName& AttributeTag, const int32 Point)
{
	if (AttributeTag == "Strength")
	{
		ApplyAttributes(StrengthPointEffect, Point);
	}
	else if (AttributeTag == "Agility")
	{
		ApplyAttributes(AgilityPointEffect, Point);
	}
	else if (AttributeTag == "Intelligence")
	{
		ApplyAttributes(IntelligencePointEffect, Point);
	}
}

void APlayerCharacter::InitAbilityActorInfo()
{
	ARPGPlayerState *RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	RPGPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(RPGPlayerState, this);
	AbilitySystemComponent = RPGPlayerState->GetAbilitySystemComponent();
	AttributeSet = RPGPlayerState->GetAttributeSet();
	OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Debuff.Stun"),
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::StunTagChanged);

	if(ARPGPlayerController *RPGPlayerController = Cast<ARPGPlayerController>(GetController()))
	{
		if(ARPGHUD *RPGHUD = Cast<ARPGHUD>(RPGPlayerController->GetHUD()))
		{
			RPGHUD->InitOverlay(RPGPlayerController, RPGPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void APlayerCharacter::ApplyAttributes(TSubclassOf<UGameplayEffect> DefaultAttributes, int32 AttributeLevel)
{
	checkf(DefaultAttributes, TEXT("未设置默认属性值，请检查蓝图"));
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultAttributes, AttributeLevel, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void APlayerCharacter::ApplyPlayerAttributes()
{
	int32 Level = GetCharacterLevel();
	const ARPGPlayerState *PS = GetPlayerState<ARPGPlayerState>();
	ApplyAttributes(DefaultPrimaryAttributes, Level);
	ApplyAttributes(DefaultSecondaryAttributes, Level);
	ApplyAttributes(DefaultVitalAttributes, Level);
	ApplyAttributes(StrengthPointEffect, PS->GetStrengthPoint());
	ApplyAttributes(AgilityPointEffect, PS->GetAgilityPoint());
	ApplyAttributes(IntelligencePointEffect, PS->GetIntelligencePoint());
}

int32 APlayerCharacter::GetCharacterLevel()
{
	const ARPGPlayerState *RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->GetPlayerLevel();
}

void APlayerCharacter::SetCharacterLevel(int32 NewLevel)
{
	ARPGPlayerState *RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	RPGPlayerState->SetPlayerLevel(NewLevel);
}

UAttributeSet* APlayerCharacter::GetAttributeSet() const
{
	const ARPGPlayerState *RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->GetAttributeSet();
}

void APlayerCharacter::Die()
{
	GetAbilitySystemComponent()->CancelAllAbilities();
	if (APlayerController *PC = Cast<APlayerController>(GetController())) PC->DisableInput(nullptr);
	Super::Die();
}

void APlayerCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeedBase * 2;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
	if (IsValid(RunNiagaraComponent)) RunNiagaraComponent->Activate();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::Walk, 3.f, false);
}

void APlayerCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeedBase;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	if (IsValid(RunNiagaraComponent)) RunNiagaraComponent->Deactivate();
}
