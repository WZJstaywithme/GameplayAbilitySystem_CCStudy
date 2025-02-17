// Copyright CC Mechanics

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputConfig.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class GASSTUDY_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	void AutoRun();

	UFUNCTION(Client,Reliable)
	void ShowDamageNumber(float DamageAmount,ACharacter* TargetCharacter,bool bBlockedHit,bool bCriticalHit);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	void ShiftPressed()
	{
		bShitKeyDown = true;
	}
	void ShiftReleased()
	{
		bShitKeyDown = false;
	}
	bool bShitKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;
	
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<USplineComponent> Spline;

	UAuraAbilitySystemComponent* GetASC();

	bool bTargeting = false;
	bool bAutoRunning = false;
	float AutoRunAcceptanceRadius = 50.0f;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	FVector CachedDestination;
};
