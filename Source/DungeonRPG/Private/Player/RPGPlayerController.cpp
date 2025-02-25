// DungeonRPG


#include "Player/RPGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "Character/PlayerCharacter.h"
#include "Input/RPGInputComponent.h"
#include "GameFramework/Character.h"
#include "Player/RPGPlayerState.h"
#include "UI/Widget/DamageTextComponent.h"
#include "UI/Widget/RestoreTextComponent.h"

ARPGPlayerController::ARPGPlayerController()
{
	bReplicates = true;
	
}

void ARPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ARPGPlayerController::ShowDamageValue_Implementation(float Damage, ACharacter* TargetCharacter, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent *DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->SetDamageText(Damage, bCriticalHit);
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

URPGAbilitySystemComponent *ARPGPlayerController::GetAbilitySystemComponent()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AbilitySystemComponent;
}

void ARPGPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() == nullptr) return;
	AbilitySystemComponent->AbilityInputPressed(InputTag);
}

void ARPGPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() == nullptr) return;
	AbilitySystemComponent->AbilityInputHeld(InputTag);
}

void ARPGPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() == nullptr) return;
	AbilitySystemComponent->AbilityInputReleased(InputTag);
}

void ARPGPlayerController::ShowRestoreValue_Implementation(float Restore, ACharacter* TargetCharacter,
	bool bCriticalHit, bool bHeal)
{
	if (IsValid(TargetCharacter) && RestoreTextComponentClass && IsLocalController())
	{
		URestoreTextComponent *RestoreTextComponent = NewObject<URestoreTextComponent>(TargetCharacter, RestoreTextComponentClass);
		RestoreTextComponent->RegisterComponent();
		RestoreTextComponent->SetRestoreText(Restore, bCriticalHit, bHeal);
		RestoreTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		RestoreTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem) Subsystem->AddMappingContext(InputContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	if(APawn* ControlledPawn = this->GetPawn<APawn>())
	{
		ForwardVector = ControlledPawn->GetActorForwardVector();
	}

	if (HasAuthority()) GetPlayerState<ARPGPlayerState>()->Init();
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	URPGInputComponent *RPGInputComponent = CastChecked<URPGInputComponent>(InputComponent);
	RPGInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Move);
	RPGInputComponent->BindAbilityActions(InputConfig, this, &ARPGPlayerController::AbilityInputPressed, &ARPGPlayerController::AbilityInputHeld, &ARPGPlayerController::AbilityInputReleased);
}

void ARPGPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if(APawn* ControlledPawn = this->GetPawn<APawn>())
	{
		if (!Cast<APlayerCharacter>(ControlledPawn)->bIsStunned)
		{
			const FVector2d InputAxisVec = InputActionValue.Get<FVector2D>();
			float Angle = atan2f(InputAxisVec.X, InputAxisVec.Y) / PI * 180.f;
			FVector Direction = ForwardVector;
			Direction = UKismetMathLibrary::RotateAngleAxis(Direction, Angle, FVector(0, 0, 1));
			ControlledPawn->AddMovementInput(Direction);
		}
	}
}
