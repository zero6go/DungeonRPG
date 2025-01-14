// DungeonRPG


#include "Player/RPGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARPGPlayerController::ARPGPlayerController()
{
	bReplicates = true;
	
}

void ARPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void ARPGPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if(ThisActor != nullptr)
		{
			if(LastActor != ThisActor)
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
		else
		{
			LastActor->UnHighlightActor();
		}
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
	InputModeData.SetHideCursorDuringCapture(true);
	SetInputMode(InputModeData);

	if(APawn* ControlledPawn = this->GetPawn<APawn>())
	{
		ForwardVector = ControlledPawn->GetActorForwardVector();
	}
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
		float Angle = atan2f(InputAxisVec.X, InputAxisVec.Y) / PI * 180.f;
		FVector Direction = ForwardVector;
		Direction = UKismetMathLibrary::RotateAngleAxis(Direction, Angle, FVector(0, 0, 1));
		ControlledPawn->AddMovementInput(Direction);
	}
}
