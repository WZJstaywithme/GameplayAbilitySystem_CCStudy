// Copyright CC Mechanics


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability -> GetCurrentActorInfo() -> IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		FPredictionKey PredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get() -> AbilityTargetDataSetDelegate(AbilitySpecHandle,PredictionKey)
		.AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		bool bCalledDelegate= AbilitySystemComponent.Get() -> CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle,PredictionKey);
		if (bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	APlayerController* PC = Ability -> GetCurrentActorInfo() -> PlayerController.Get();
	FHitResult Hit;
	PC -> GetHitResultUnderCursor(ECC_Visibility,false,Hit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data -> HitResult = Hit;
	DataHandle.Add(Data);

	AbilitySystemComponent -> ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		 FGameplayTag(),
		 AbilitySystemComponent -> ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& AbilityHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent -> ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(AbilityHandle);
	}
}

