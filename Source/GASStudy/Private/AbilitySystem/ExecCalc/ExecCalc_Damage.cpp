// Copyright CC Mechanics


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	//1.声明属性捕捉
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		//2.定义属性捕捉
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance,Target,false);

		FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_Armor,ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_ArmorPenetration,ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_BlockChance,BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitChance,CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitResistance,CriticalHitResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitDamage,CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Fire,FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Lightning,LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Arcane,ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Physical,PhysicalResistanceDef);

	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DamageStatics;
	return DamageStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//3.注册属性捕捉
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
	
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatarActor = SourceASC->GetAvatarActor();
	AActor* TargetAvatarActor = TargetASC->GetAvatarActor();
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatarActor);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatarActor);
	
	FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float Damage = 0.f;
	for (auto& Pair: FAuraGameplayTags::Get().DamageTypesToResistance)
	{
		FGameplayTag DamageTypeTag = Pair.Key;
		FGameplayTag ResistanceTag = Pair.Value;

		FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParams, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;
		Damage += DamageTypeValue;
	}
	
	float BlockChance = 0.f;
	//4.调用方法获取属性值
	//格挡几率
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParams,BlockChance);
	BlockChance = FMath::Max<float>(0.f,BlockChance);
	float RandomValue = FMath::RandRange(1,100);
	Damage = BlockChance >= RandomValue ? Damage / 2 : Damage;

	FGameplayEffectContextHandle GameplayEffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(GameplayEffectContextHandle,BlockChance >= RandomValue);

	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatarActor);
	UCurveTable* CurveTable = CharacterClassInfo ->DamageCalculationCoefficients;
	
	FRealCurve* ArmorPenetrationCurve = CurveTable->FindCurve(FName("ArmorPenetration"),FString(""));
	float ArmorPenetrationCoefficients = ArmorPenetrationCurve -> Eval(SourceCombatInterface -> GetPlayerLevel());

	FRealCurve* EffectiveArmorCurve = CurveTable->FindCurve(FName("EffectiveArmor"),FString(""));
	float EffectiveArmorCoefficients = EffectiveArmorCurve -> Eval(TargetCombatInterface -> GetPlayerLevel());
	
	// 护甲计算,通过护甲和护甲穿透减少收到的伤害
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParams,TargetArmor);
	TargetArmor = FMath::Max<float>(0.f,TargetArmor);
	
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParams,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f,SourceArmorPenetration);
	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficients) / 100.f;
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficients) / 100.f;

	// 是否暴击
	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParams,CriticalHitChance);
	CriticalHitChance = FMath::Max<float>(0.f,CriticalHitChance);
	
	float CriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParams,CriticalHitResistance);
	CriticalHitResistance = FMath::Max<float>(0.f,CriticalHitResistance);
	
	float CriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParams,CriticalHitDamage);
	CriticalHitDamage = FMath::Max<float>(0.f,CriticalHitDamage);

	FRealCurve* CriticalHitResistanceCurve = CurveTable->FindCurve(FName("CriticalHitResistance"),FString(""));
	float CriticalHitResistanceCoefficients = CriticalHitResistanceCurve -> Eval(TargetCombatInterface -> GetPlayerLevel());
	
	const float EffectCriticalHitResistance = CriticalHitChance - CriticalHitResistance * CriticalHitResistanceCoefficients;
	float CriticalRandomValue = FMath::RandRange(1,100);
	Damage = EffectCriticalHitResistance >= CriticalRandomValue ? Damage * 2 : Damage;
	UAuraAbilitySystemLibrary::SetIsCriticalHit(GameplayEffectContextHandle,EffectCriticalHitResistance >= CriticalRandomValue);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
