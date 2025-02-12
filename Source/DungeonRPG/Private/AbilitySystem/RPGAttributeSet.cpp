// DungeonRPG


#include "AbilitySystem/RPGAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Character/CharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "Player/RPGPlayerController.h"
#include "GameFramework/Character.h"
#include "Player/RPGPlayerState.h"

URPGAttributeSet::URPGAttributeSet()
{
}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Agility, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, CriticalRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MagicPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);
	
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

}

void URPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp<float>(GetHealth(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp<float>(GetMana(), 0.0f, GetMaxMana()));
	}
	//计算物理伤害
	if (Data.EvaluatedData.Attribute == GetIncomingAttackDamageAttribute())
	{
		float LocalIncomingAttackDamage = GetIncomingAttackDamage();
		SetIncomingAttackDamage(0.f);
		if (LocalIncomingAttackDamage > 0)
		{
			ACharacterBase *Target;
			if (ARPGPlayerState *PS= Cast<ARPGPlayerState>(GetOwningActor()))  Target = Cast<ACharacterBase>(PS->GetPawn());
			else Target = Cast<ACharacterBase>(GetOwningActor());
			
			ACharacterBase *Source = Cast<ACharacterBase>(Data.EffectSpec.GetContext().GetInstigatorAbilitySystemComponent()->GetAvatarActor());
			//自残或受到场地伤害（EffectActor的ASC是触发该Actor的玩家所拥有的ASC，详见RPGEffectActor代码）
			if (Source == Target)
			{
				URPGAttributeSet *TargetAttributeSet = Cast<URPGAttributeSet>(Target->GetAttributeSet());
				float ArmorValue = TargetAttributeSet->GetArmor();
				ArmorValue = ArmorValue / (100.f + ArmorValue);
				LocalIncomingAttackDamage = LocalIncomingAttackDamage * (1.f - ArmorValue);
				const float NewHealth = GetHealth() - LocalIncomingAttackDamage;
				SetHealth(FMath::Clamp<float>(NewHealth, 0.0f, GetMaxHealth()));
			
				const bool bFatal = NewHealth <= 0.f;
				if (bFatal)
				{
					ICombatInterface *CombatInterface = Cast<ICombatInterface>(Target);
					if (CombatInterface)
					{
						CombatInterface->Die();
					}
				}
				//显示伤害值
				if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Target->Controller))
				{
					PC->ShowDamageValue(LocalIncomingAttackDamage, Cast<ACharacter>(Target), false);
				}
			}
			//受到其他Character伤害
			else
			{
				URPGAttributeSet *SourceAttributeSet = Cast<URPGAttributeSet>(Source->GetAttributeSet());
				URPGAttributeSet *TargetAttributeSet = Cast<URPGAttributeSet>(Target->GetAttributeSet());

				float ArmorValue = TargetAttributeSet->GetArmor();
				ArmorValue = ArmorValue / (100.f + ArmorValue);
				LocalIncomingAttackDamage = LocalIncomingAttackDamage * (1.f - ArmorValue);
			
				float CritRate = SourceAttributeSet->GetCriticalRate();
				float CritDmg = SourceAttributeSet->GetCriticalDamage() / 100.f;
				const float Seed = FMath::RandRange(0.f, 100.f);
				const bool bCriticalHit = Seed == 100.f ? true : Seed < CritRate;
				if (bCriticalHit) LocalIncomingAttackDamage *= CritDmg;
			
				const float NewHealth = GetHealth() - LocalIncomingAttackDamage;
				SetHealth(FMath::Clamp<float>(NewHealth, 0.0f, GetMaxHealth()));
			
				const bool bFatal = NewHealth <= 0.f;
				if (bFatal)
				{
					ICombatInterface *CombatInterface = Cast<ICombatInterface>(Target);
					if (CombatInterface)
					{
						CombatInterface->Die();
					}
				}
				else
				{
					FGameplayTagContainer TagContainer;
					TagContainer.AddTag(FGameplayTag::RequestGameplayTag("Effects.HitReact"));
					GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
				}
				//显示伤害值
				if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Source->Controller))
				{
					PC->ShowDamageValue(LocalIncomingAttackDamage, Cast<ACharacter>(Target), bCriticalHit);
				}
				if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Target->Controller))
				{
					PC->ShowDamageValue(LocalIncomingAttackDamage, Cast<ACharacter>(Target), bCriticalHit);
				}
			}
		}
	}
	//计算魔法伤害
	if (Data.EvaluatedData.Attribute == GetIncomingMagicDamageAttribute())
	{
		float LocalIncomingMagicDamage = GetIncomingMagicDamage();
		SetIncomingMagicDamage(0.f);
		if (LocalIncomingMagicDamage > 0)
		{
			ACharacterBase *Target;
			if (ARPGPlayerState *PS= Cast<ARPGPlayerState>(GetOwningActor()))  Target = Cast<ACharacterBase>(PS->GetPawn());
			else Target = Cast<ACharacterBase>(GetOwningActor());
			
			ACharacterBase *Source = Cast<ACharacterBase>(Data.EffectSpec.GetContext().GetInstigatorAbilitySystemComponent()->GetAvatarActor());
			//自残或受到场地伤害（EffectActor的ASC是触发该Actor的玩家所拥有的ASC，详见RPGEffectActor代码）
			if (Source == Target)
			{
				URPGAttributeSet *TargetAttributeSet = Cast<URPGAttributeSet>(Target->GetAttributeSet());
				float MagicRes = TargetAttributeSet->GetMagicResistance();
				MagicRes = MagicRes / (100.f + MagicRes);
				LocalIncomingMagicDamage = LocalIncomingMagicDamage * (1.f - MagicRes);
				const float NewHealth = GetHealth() - LocalIncomingMagicDamage;
				SetHealth(FMath::Clamp<float>(NewHealth, 0.0f, GetMaxHealth()));
			
				const bool bFatal = NewHealth <= 0.f;
				if (bFatal)
				{
					ICombatInterface *CombatInterface = Cast<ICombatInterface>(Target);
					if (CombatInterface)
					{
						CombatInterface->Die();
					}
				}
				//显示伤害值
				if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Target->Controller))
				{
					PC->ShowDamageValue(LocalIncomingMagicDamage, Cast<ACharacter>(Target), false);
				}
			}
			//受到其他Character伤害
			else
			{
				URPGAttributeSet *SourceAttributeSet = Cast<URPGAttributeSet>(Source->GetAttributeSet());
				URPGAttributeSet *TargetAttributeSet = Cast<URPGAttributeSet>(Target->GetAttributeSet());

				float MagicPow = SourceAttributeSet->GetMagicPower() / 100.f;
				float MagicRes = TargetAttributeSet->GetMagicResistance();
				MagicRes = MagicRes / (100.f + MagicRes);
				LocalIncomingMagicDamage = LocalIncomingMagicDamage * MagicPow * (1.f - MagicRes);
			
				float CritRate = SourceAttributeSet->GetCriticalRate();
				float CritDmg = SourceAttributeSet->GetCriticalDamage() / 100.f;
				const float Seed = FMath::RandRange(0.f, 100.f);
				const bool bCriticalHit = Seed == 100.f ? true : Seed < CritRate;
				if (bCriticalHit) LocalIncomingMagicDamage *= CritDmg;
			
				const float NewHealth = GetHealth() - LocalIncomingMagicDamage;
				SetHealth(FMath::Clamp<float>(NewHealth, 0.0f, GetMaxHealth()));
			
				const bool bFatal = NewHealth <= 0.f;
				if (bFatal)
				{
					ICombatInterface *CombatInterface = Cast<ICombatInterface>(Target);
					if (CombatInterface)
					{
						CombatInterface->Die();
					}
				}
				else
				{
					FGameplayTagContainer TagContainer;
					TagContainer.AddTag(FGameplayTag::RequestGameplayTag("Effects.HitReact"));
					GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
				}
				//显示伤害值
				if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Source->Controller))
				{
					PC->ShowDamageValue(LocalIncomingMagicDamage, Cast<ACharacter>(Target), bCriticalHit);
				}
				if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Target->Controller))
				{
					PC->ShowDamageValue(LocalIncomingMagicDamage, Cast<ACharacter>(Target), bCriticalHit);
				}
			}
		}
	}

	//回血回蓝
	if (Data.EvaluatedData.Attribute == GetIncomingHealAttribute())
	{
		float LocalIncomingHeal = GetIncomingHeal();
		SetIncomingHeal(0.f);
		if (LocalIncomingHeal > 0)
		{
			ACharacterBase *Target;
			if (ARPGPlayerState *PS= Cast<ARPGPlayerState>(GetOwningActor()))  Target = Cast<ACharacterBase>(PS->GetPawn());
			else Target = Cast<ACharacterBase>(GetOwningActor());
			ACharacterBase *Source = Cast<ACharacterBase>(Data.EffectSpec.GetContext().GetInstigatorAbilitySystemComponent()->GetAvatarActor());
			URPGAttributeSet *SourceAttributeSet = Cast<URPGAttributeSet>(Source->GetAttributeSet());
			URPGAttributeSet *TargetAttributeSet = Cast<URPGAttributeSet>(Target->GetAttributeSet());
			
			float CritRate = SourceAttributeSet->GetCriticalRate();
			float CritDmg = SourceAttributeSet->GetCriticalDamage() / 100.f;
			const float Seed = FMath::RandRange(0.f, 100.f);
			const bool bCriticalHit = Seed == 100.f ? true : Seed < CritRate;
			if (bCriticalHit) LocalIncomingHeal *= CritDmg;
			
			const float NewHealth = GetHealth() + LocalIncomingHeal;
			SetHealth(FMath::Clamp<float>(NewHealth, 0.0f, GetMaxHealth()));
			
			//显示回复量
			if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Source->Controller))
			{
				PC->ShowRestoreValue(LocalIncomingHeal, Cast<ACharacter>(Target), bCriticalHit, true);
			}
			if (Source != Target)
			{
				if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Target->Controller))
				{
					PC->ShowRestoreValue(LocalIncomingHeal, Cast<ACharacter>(Target), bCriticalHit, true);
				}
			}
		}
	}
	if (Data.EvaluatedData.Attribute == GetIncomingManaAttribute())
	{
		float LocalIncomingMana = GetIncomingMana();
		SetIncomingMana(0.f);
		if (LocalIncomingMana > 0)
		{
			ACharacterBase *Target;
			if (ARPGPlayerState *PS= Cast<ARPGPlayerState>(GetOwningActor()))  Target = Cast<ACharacterBase>(PS->GetPawn());
			else Target = Cast<ACharacterBase>(GetOwningActor());
			ACharacterBase *Source = Cast<ACharacterBase>(Data.EffectSpec.GetContext().GetInstigatorAbilitySystemComponent()->GetAvatarActor());
			URPGAttributeSet *SourceAttributeSet = Cast<URPGAttributeSet>(Source->GetAttributeSet());
			URPGAttributeSet *TargetAttributeSet = Cast<URPGAttributeSet>(Target->GetAttributeSet());
			
			float CritRate = SourceAttributeSet->GetCriticalRate();
			float CritDmg = SourceAttributeSet->GetCriticalDamage() / 100.f;
			const float Seed = FMath::RandRange(0.f, 100.f);
			const bool bCriticalHit = Seed == 100.f ? true : Seed < CritRate;
			if (bCriticalHit) LocalIncomingMana *= CritDmg;
			
			const float NewMana = GetMana() + LocalIncomingMana;
			SetMana(FMath::Clamp<float>(NewMana, 0.0f, GetMaxMana()));
			
			//显示回复量
			if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Source->Controller))
			{
				PC->ShowRestoreValue(LocalIncomingMana, Cast<ACharacter>(Target), bCriticalHit, false);
			}
			if (Source != Target)
			{
				if (ARPGPlayerController *PC = Cast<ARPGPlayerController>(Target->Controller))
				{
					PC->ShowRestoreValue(LocalIncomingMana, Cast<ACharacter>(Target), bCriticalHit, false);
				}
			}
		}
	}
}

void URPGAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Strength, OldStrength);
}

void URPGAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Agility, OldAgility);
}

void URPGAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Intelligence, OldIntelligence);
}

void URPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health, OldHealth);
}

void URPGAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Mana, OldMana);
}

void URPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHealth, OldMaxHealth);
}

void URPGAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AttackPower, OldAttackPower);
}

void URPGAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Armor, OldArmor);
}

void URPGAttributeSet::OnRep_CriticalRate(const FGameplayAttributeData& OldCriticalRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CriticalRate, OldCriticalRate);
}

void URPGAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CriticalDamage, CriticalDamage);
}

void URPGAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxMana, OldMaxMana);
}

void URPGAttributeSet::OnRep_MagicPower(const FGameplayAttributeData& OldMagicPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MagicPower, OldMagicPower);
}

void URPGAttributeSet::OnRep_MagicResistance(const FGameplayAttributeData& OldMagicResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MagicResistance, OldMagicResistance);
}
