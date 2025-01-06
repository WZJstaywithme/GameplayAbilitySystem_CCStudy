// Copyright CC Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	/** enemy interface **/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** enemy interface end **/

	/** combat interface **/
	virtual int32 GetPlayerLevel() override;
	/** combat interface end **/

	
	UPROPERTY(BlueprintReadOnly)
	bool bHighLighted = false;

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;
};
