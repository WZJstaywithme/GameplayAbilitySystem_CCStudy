// Copyright CC Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** combat interface **/
	virtual int32 GetPlayerLevel() override;
	/** combat interface end **/

private:
	virtual void InitAbilityActorInfo() override;
};
