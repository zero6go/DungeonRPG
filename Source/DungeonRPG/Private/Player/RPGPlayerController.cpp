// DungeonRPG


#include "Player/RPGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ARPGPlayerController::ARPGPlayerController()
{
	bReplicates = true;
	
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(InputContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(true);
	SetInputMode(InputModeData);
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Move);
}

void ARPGPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if(APawn* ControlledPawn = this->GetPawn<APawn>())
	{
		const FVector2d InputAxisVec = InputActionValue.Get<FVector2D>();
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), InputAxisVec.Y);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), InputAxisVec.X);
	}
}
