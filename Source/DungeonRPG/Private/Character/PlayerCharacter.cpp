// DungeonRPG


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/RPGPlayerController.h"
#include "Player/RPGPlayerState.h"
#include "UI/HUD/RPGHUD.h"

APlayerCharacter::APlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

//服务器
void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
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

void APlayerCharacter::InitAbilityActorInfo()
{
	ARPGPlayerState *RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	RPGPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(RPGPlayerState, this);
	AbilitySystemComponent = RPGPlayerState->GetAbilitySystemComponent();
	AttributeSet = RPGPlayerState->GetAttributeSet();

	if(ARPGPlayerController *RPGPlayerController = Cast<ARPGPlayerController>(GetController()))
	{
		if(ARPGHUD *RPGHUD = Cast<ARPGHUD>(RPGPlayerController->GetHUD()))
		{
			RPGHUD->InitOverlay(RPGPlayerController, RPGPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	int32 Level = GetCharacterLevel();
	InitDefaultAttributes(DefaultPrimaryAttributes, Level);
	InitDefaultAttributes(DefaultSecondaryAttributes, Level);
	InitDefaultAttributes(DefaultVitalAttributes, Level);
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
	if (APlayerController *PC = Cast<APlayerController>(GetController())) PC->DisableInput(nullptr);
	Super::Die();
}
