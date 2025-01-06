// Copyright CC Mechanics


#include "UI/WidgetController/AuraUserWidgetController.h"

#include "MovieSceneFwd.h"

void UAuraUserWidgetController::SetWidgetControllerParams(const FWidgetControllerParams WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UAuraUserWidgetController::BindCallbackToDependencies()
{
	
}
