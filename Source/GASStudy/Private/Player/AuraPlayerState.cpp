// Copyright CC Mechanics


#include "Player/AuraPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
	AbilitySystem -> SetIsReplicated(true);
	AbilitySystem -> SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystem; 
}
