// DungeonRPG


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
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
	for(auto Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability.AbilityClass, Ability.Level);
		if (URPGGameplayAbility *RPGAbility = Cast<URPGGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(RPGAbility->InputTag);
			GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		}
	}
}

//客户端
void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
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
