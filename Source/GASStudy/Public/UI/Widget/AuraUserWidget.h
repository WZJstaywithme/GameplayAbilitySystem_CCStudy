// Copyright CC Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "UserWidget")
	void WidgetControllerSet();

public:
	UPROPERTY(BlueprintReadOnly, Category = "UserWidget")
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable, Category = "UserWidget")
	void SetWidgetController(UObject* InWidgetController);
};
