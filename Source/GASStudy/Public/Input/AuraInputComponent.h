// Copyright CC Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserClass,typename PressedFuncType,typename ReleasedFuncType,typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig,UserClass* Object,PressedFuncType PressedFunc,ReleasedFuncType ReleasedFunc,HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (FAuraInputAction AbilityInputAction : InputConfig -> AbilityInputActions)
	{
		if (AbilityInputAction.InputAction && AbilityInputAction.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Started,Object,PressedFunc,AbilityInputAction.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Completed,Object,ReleasedFunc,AbilityInputAction.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Triggered,Object,HeldFunc,AbilityInputAction.InputTag);
			}
		}
	}
}
