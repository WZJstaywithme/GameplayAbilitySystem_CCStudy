// Copyright CC Mechanics

#include "Character/AuraCharacterBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon -> SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

int32 AAuraCharacterBase::GetPlayerLevel()
{
	return ICombatInterface::GetPlayerLevel();
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.f);
}


void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const\
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);

	FGameplayEffectContextHandle GameplayEffectContextHandle = GetAbilitySystemComponent() -> MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent() -> MakeOutgoingSpec(GameplayEffectClass,Level,GameplayEffectContextHandle);
	GetAbilitySystemComponent() -> ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(),GetAbilitySystemComponent());
}

