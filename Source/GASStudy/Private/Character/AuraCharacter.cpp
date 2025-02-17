// Copyright CC Mechanics


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent -> bOrientRotationToMovement = true;
	MovementComponent -> RotationRate = FRotator(0, 400, 0);
	MovementComponent -> bConstrainToPlane = true;
	MovementComponent -> bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState -> GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()	
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState -> GetAbilitySystemComponent() -> InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState -> GetAbilitySystemComponent()) -> AbilityActorInfoSet();

	AbilitySystem = AuraPlayerState -> GetAbilitySystemComponent();;
	AttributeSet = AuraPlayerState -> GetAuraAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD -> InitOverlay(AuraPlayerController,AuraPlayerState,AbilitySystem,AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
