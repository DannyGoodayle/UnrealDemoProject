#include "Character/GameplayEffectExecution_Damage.h"

#include "Character/Attributes/StandardAttributeSet.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_SETBYCALLER_DAMAGE, "SetByCaller.Damage");

struct DamageExecutionCalculationStats
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageOutgoingMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageIncomingMultiplier);

	DamageExecutionCalculationStats()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStandardAttributeSet, DamageOutgoingMultiplier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStandardAttributeSet, DamageIncomingMultiplier, Target, true);
	}
};

static const DamageExecutionCalculationStats& GetDamageCalculationStats()
{
	static DamageExecutionCalculationStats sStatics;
	return sStatics;
}

UGameplayEffectExecution_Damage::UGameplayEffectExecution_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageCalculationStats().DamageOutgoingMultiplierDef);
	RelevantAttributesToCapture.Add(GetDamageCalculationStats().DamageIncomingMultiplierDef);
}

void UGameplayEffectExecution_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& executionParams,
	FGameplayEffectCustomExecutionOutput& outExecutionOutput) const
{
	const FGameplayEffectSpec& spec = executionParams.GetOwningSpec();
	const float setByCallerDamage = spec.GetSetByCallerMagnitude(TAG_SETBYCALLER_DAMAGE, true, 0);

	float damageDealt = setByCallerDamage;
	
	const FGameplayTagContainer* sourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluateParameters;
	evaluateParameters.SourceTags = sourceTags;
	evaluateParameters.TargetTags = targetTags;
	
	float outgoingDamageMultiplier = 1.0f;
	if (executionParams.AttemptCalculateCapturedAttributeMagnitude(DamageExecutionCalculationStats().DamageOutgoingMultiplierDef, evaluateParameters, outgoingDamageMultiplier))
	{
		damageDealt *= outgoingDamageMultiplier;
	}
	
	float incomingDamageMultiplier = 1.0f;
	if (executionParams.AttemptCalculateCapturedAttributeMagnitude(DamageExecutionCalculationStats().DamageIncomingMultiplierDef, evaluateParameters, incomingDamageMultiplier))
	{
		damageDealt *= incomingDamageMultiplier;
	}
	
	if(damageDealt > 0)
	{
		outExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UStandardAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, damageDealt));
	}
}
