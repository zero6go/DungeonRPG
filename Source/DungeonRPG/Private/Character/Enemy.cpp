// DungeonRPG


#include "Character/Enemy.h"

#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "AI/RPGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/RPGUserWidget.h"

AEnemy::AEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
}


void AEnemy::Die()
{
	SetLifeSpan(5.f);
	RPGAIController->GetBlackboardComponent()->SetValueAsBool("bAlive", false);
	
	Super::Die();
}

void AEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	if (HasAuthority()) RPGAIController->GetBlackboardComponent()->SetValueAsBool("bStunned", bIsStunned);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	
	if (HasAuthority())
	{
		URPGAbilitySystemComponent *ASC = Cast<URPGAbilitySystemComponent>(AbilitySystemComponent);
		for(auto Ability : StartupAbilities)
		{
			Ability.Level = GetCharacterLevel() / 10.f + 1;
			ASC->GiveCharacterAbility(Ability);
		}
	}

	if (URPGUserWidget *HealthBarWidget = CastChecked<URPGUserWidget>(HealthBar->GetWidget()))
	{
		HealthBarWidget->SetWidgetController(this);
	}
	
	if(const URPGAttributeSet *AS = CastChecked<URPGAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this, AS](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(AS->GetHealth());
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this, AS](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
			});

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Effects.HitReact"),
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemy::HitReactTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Debuff.Stun"),
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemy::StunTagChanged);
		
		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}
	
}

void AEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : WalkSpeedBase;
	if (HasAuthority()) RPGAIController->GetBlackboardComponent()->SetValueAsBool("bHitReacting", bHitReacting);
}

void AEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		ApplyAttributes(DefaultPrimaryAttributes, Level);
		ApplyAttributes(DefaultSecondaryAttributes, Level);
		ApplyAttributes(DefaultVitalAttributes, Level);
	}
}

void AEnemy::ApplyAttributes(TSubclassOf<UGameplayEffect> DefaultAttributes, int32 AttributeLevel)
{
	checkf(DefaultAttributes, TEXT("未设置默认属性值，请检查蓝图"));
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultAttributes, AttributeLevel, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

int32 AEnemy::GetCharacterLevel()
{
	return Level;
}

void AEnemy::SetCharacterLevel(int32 NewLevel)
{
	Level = NewLevel;
}

void AEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	RPGAIController = Cast<ARPGAIController>(NewController);

	RPGAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	RPGAIController->GetBlackboardComponent()->SetValueAsBool("bHitReacting", false);
	RPGAIController->GetBlackboardComponent()->SetValueAsBool("bAlive", true);
	RPGAIController->RunBehaviorTree(BehaviorTree);
}
