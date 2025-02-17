// Copyright CC Mechanics


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// UKismetSystemLibrary::PrintString(this,FString("ActivateAbility（C++）"),true,true,FLinearColor::Yellow,3.f);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	bool bHasAuth = GetAvatarActorFromActorInfo() -> HasAuthority();
	if (!bHasAuth) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (CombatInterface)
	{
		FVector CombatSocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - CombatSocketLocation).Rotation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(CombatSocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AAuraProjectile* AuraProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

		FGameplayEffectContextHandle EffectContextHandle = SourceASC -> MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(AuraProjectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(AuraProjectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult Hit;
		EffectContextHandle.AddHitResult(Hit);

		FGameplayEffectSpecHandle SpecHandle = SourceASC -> MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),SourceASC -> MakeEffectContext());

		for (auto& Pair : DamageTypes)
		{
			float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key,ScaledDamage);
		}
		AuraProjectile -> DamageSpecHandle = SpecHandle;
		AuraProjectile->FinishSpawning(SpawnTransform);
	}
}
