// Copyright CC Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "UI/WidgetController/AuraUserWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegate,const FAuraAttributeInfo&, Info);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GASSTUDY_API UAttributeMenuWidgetController : public UAuraUserWidgetController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	virtual void BindCallbackToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeInfoDelegate AttributeInfoDelegate;

	void BroadcastAttributeInfo(const FGameplayTag& Tag,const FGameplayAttribute& Attribute) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
