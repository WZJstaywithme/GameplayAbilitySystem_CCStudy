// Copyright CC Mechanics

#include "UI/HUD/AuraHUD.h"

void AAuraHUD::InitOverlay(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("Overlay Widget class is null"));
	checkf(OverlayWidgetControllerClass,TEXT("Overlay Widget class is null"));
	
	UUserWidget* UserWidget = CreateWidget(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(UserWidget);
	
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	
	UserWidget -> AddToViewport();
}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController -> SetWidgetControllerParams(WCParams);
	}
	return OverlayWidgetController;
}
