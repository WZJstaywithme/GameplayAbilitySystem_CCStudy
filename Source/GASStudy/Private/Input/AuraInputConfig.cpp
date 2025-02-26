// Copyright CC Mechanics


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{

	for (FAuraInputAction AbilityInputAction : AbilityInputActions)
	{
		if (AbilityInputAction.InputAction && AbilityInputAction.InputTag == InputTag)
		{
			return AbilityInputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can`t find AbilityInputAction For InputTag [%s], on InputConfig [%s]"),*InputTag.ToString(),*GetNameSafe(this));
	}

	return nullptr;
}
