// DungeonRPG


#include "Player/RPGPlayerState.h"

#include "AbilitySystem/LevelUpInfo.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Character/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

ARPGPlayerState::ARPGPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void ARPGPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPGPlayerState, Level);
	DOREPLIFETIME(ARPGPlayerState, XP);
	DOREPLIFETIME(ARPGPlayerState, AttributePoint);
	DOREPLIFETIME(ARPGPlayerState, SpellPoint);
	DOREPLIFETIME(ARPGPlayerState, StrengthPoint);
	DOREPLIFETIME(ARPGPlayerState, AgilityPoint);
	DOREPLIFETIME(ARPGPlayerState, IntelligencePoint);
}

UAbilitySystemComponent* ARPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARPGPlayerState::SetPlayerLevel(int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChanged.Broadcast(NewLevel);
}

void ARPGPlayerState::LevelUp(int32 OldLevel, int32 NewLevel)
{
	float AttributePointGet = LevelUpInfo->GetLevelUpAttributePointReward(OldLevel, NewLevel);
	float SpellPointGet = LevelUpInfo->GetLevelUpSpellPointReward(OldLevel, NewLevel);
	AddAttributePoint(AttributePointGet);
	AddSpellPoint(SpellPointGet);
	SetPlayerLevel(NewLevel);
	Cast<APlayerCharacter>(GetPawn())->NetMulticastLevelUp();
	Cast<URPGAbilitySystemComponent>(AbilitySystemComponent)->UpdateAbilityStatusUnlockable(OldLevel, NewLevel);
}

void ARPGPlayerState::SetPlayerXP(int32 NewXP)
{
	XP = NewXP;
	OnXPChanged.Broadcast(NewXP);
	if (XP >= NextLevelXPRequire)
	{
		int32 NewLevel = LevelUpInfo->FindLevelByXP(XP);
		NextLevelXPRequire = LevelUpInfo->GetRequireXPByLevel(NewLevel + 1);
		LevelUp(Level, NewLevel);
	}
}

void ARPGPlayerState::AddXP(float XPRewardRate, int32 EnemyLV)
{
	const int32 XPReward = 5 * (EnemyLV + 1) * EnemyLV * XPRewardRate;
	SetPlayerXP(XP + XPReward);
}

void ARPGPlayerState::AddAttributePoint(int32 InAttributePoint)
{
	AttributePoint += InAttributePoint;
	OnAttributePointChanged.Broadcast(AttributePoint);
}

void ARPGPlayerState::UpgradeAttribute(const FName &AttributeTag)
{
	if (AttributePoint == 0) return;
	AttributePoint--;
	OnAttributePointChanged.Broadcast(AttributePoint);
	APlayerCharacter *Player = Cast<APlayerCharacter>(GetPawn());
	if (AttributeTag == "Strength")
	{
		AddStrengthPoint(1);
		Player->UpgradeAttribute(AttributeTag, 1);
	}
	else if (AttributeTag == "Agility")
	{
		AddAgilityPoint(1);
		Player->UpgradeAttribute(AttributeTag, 1);
	}
	else if (AttributeTag == "Intelligence")
	{
		AddIntelligencePoint(1);
		Player->UpgradeAttribute(AttributeTag, 1);
	}
}

void ARPGPlayerState::AddSpellPoint(int32 InSpellPoint)
{
	SpellPoint += InSpellPoint;
	OnSpellPointChanged.Broadcast(SpellPoint);
}

void ARPGPlayerState::SpellLevelUp(const FGameplayTag& AbilityTag)
{
	if (SpellPoint == 0) return;
	SpellPoint--;
	OnSpellPointChanged.Broadcast(SpellPoint);
	Cast<URPGAbilitySystemComponent>(AbilitySystemComponent)->ServerSpellLevelUp(AbilityTag);
}

void ARPGPlayerState::AddStrengthPoint(int32 InStrengthPoint)
{
	StrengthPoint += InStrengthPoint;
}

void ARPGPlayerState::AddAgilityPoint(int32 InAgilityPoint)
{
	AgilityPoint += InAgilityPoint;
}

void ARPGPlayerState::AddIntelligencePoint(int32 InIntelligencePoint)
{
	IntelligencePoint += InIntelligencePoint;
}

void ARPGPlayerState::Init()
{
	checkf(LevelUpInfo, TEXT("PlayerState未设置LevelUpInfo"));
	NextLevelXPRequire = LevelUpInfo->GetRequireXPByLevel(Level + 1);
}

void ARPGPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChanged.Broadcast(Level);
}

void ARPGPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChanged.Broadcast(XP);
}

void ARPGPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
	OnAttributePointChanged.Broadcast(AttributePoint);
}

void ARPGPlayerState::OnRep_SpellPoint(int32 OldSpellPoint)
{
	OnSpellPointChanged.Broadcast(SpellPoint);
}
