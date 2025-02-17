// Copyright CC Mechanics

#include "Character/AuraCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "GASStudy/GASStudy.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent() -> SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent() -> SetGenerateOverlapEvents(false);
	GetMesh() -> SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh() -> SetCollisionResponseToChannel(EEC_Projectile,ECR_Overlap);
	GetMesh() -> SetGenerateOverlapEvents(true);
	
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

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon -> DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	MulticastHandleDeath_Implementation();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon -> SetSimulatePhysics(true);
	Weapon -> SetEnableGravity(true);
	Weapon -> SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh() -> SetSimulatePhysics(true);
	GetMesh() -> SetEnableGravity(true);
	GetMesh() -> SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh() -> SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	GetCapsuleComponent() -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}


// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	return  Weapon -> GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh() -> SetMaterial(0, MaterialInstance);
		StartDissolveTimeline(MaterialInstance);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* WeaponMaterialInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance,this);
		Weapon -> SetMaterial(0, WeaponMaterialInstance);
		WeaponStartDissolveTimeline(WeaponMaterialInstance);
	}
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if(!HasAuthority()) return;

	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystem);
	AuraAbilitySystemComponent -> AddCharacterAbilities(StartupAbilities);
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

