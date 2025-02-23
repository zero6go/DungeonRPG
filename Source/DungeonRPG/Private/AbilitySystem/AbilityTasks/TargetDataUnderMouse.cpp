// DungeonRPG


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "DungeonRPG/DungeonRPG.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::GetTargetDataUnderMouse(UGameplayAbility *OwningAbility)
{
	UTargetDataUnderMouse *Obj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return Obj;
}

void UTargetDataUnderMouse::Activate()
{
	if (Ability->GetActorInfo().IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(),
			GetActivationPredictionKey()).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());
		if (bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	APlayerController *PC = Ability->GetActorInfo().PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Target), false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit *Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(), DataHandle,
		FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle &DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
