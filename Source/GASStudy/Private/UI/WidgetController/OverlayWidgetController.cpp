// Copyright CC Mechanics


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet -> GetHealth());
	OnMaxMaxHealthChanged.Broadcast(AuraAttributeSet -> GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet -> GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet -> GetMaxMana());
}

void UOverlayWidgetController::BindCallbackToDependencies()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent -> GetGameplayAttributeValueChangeDelegate(AuraAttributeSet -> GetHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data){OnHealthChanged.Broadcast(Data.NewValue);});

	AbilitySystemComponent -> GetGameplayAttributeValueChangeDelegate(AuraAttributeSet -> GetMaxHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data){OnMaxMaxHealthChanged.Broadcast(Data.NewValue);});

	AbilitySystemComponent -> GetGameplayAttributeValueChangeDelegate(AuraAttributeSet -> GetManaAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data){OnManaChanged.Broadcast(Data.NewValue);});
 
	AbilitySystemComponent -> GetGameplayAttributeValueChangeDelegate(AuraAttributeSet -> GetMaxManaAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data){OnMaxManaChanged.Broadcast(Data.NewValue);});

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent) -> EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// "Message.HealthPotion".MatchTag("Message") will return true
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					MessageRowWidgetRowDelegate.Broadcast(*Row);
				}
			}
		});
}