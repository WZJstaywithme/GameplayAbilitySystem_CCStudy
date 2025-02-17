// Copyright CC Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FValidDataSignture,FGameplayAbilityTargetDataHandle,DataHandle);

/**
 * 
 */
UCLASS()
class GASSTUDY_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,Category="Ability|Tasks",meta=(DisplayName="TargetDataUnderMouse",HidePin="OwningAbility",DefaultToSelf="OwningAbility",BlueprintInternalUseOnly=true))
	static  UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FValidDataSignture ValidData;

private:

	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& AbilityHandle, FGameplayTag ActivationTag);
};
