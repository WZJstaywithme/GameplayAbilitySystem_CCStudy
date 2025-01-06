// Copyright CC Mechanics


#include "Character/AuraEnemy.h"

#include "GASStudy/GASStudy.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh() -> SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystem = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystem");
	AbilitySystem -> SetIsReplicated(true);
	AbilitySystem -> SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::HighlightActor()
{
	GetMesh() -> SetRenderCustomDepth(true);
	GetMesh() -> SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon -> SetRenderCustomDepth(true);
	GetMesh() -> SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh() -> SetRenderCustomDepth(false);
	Weapon -> SetRenderCustomDepth(false);

}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystem -> InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystem) -> AbilityActorInfoSet();
}
