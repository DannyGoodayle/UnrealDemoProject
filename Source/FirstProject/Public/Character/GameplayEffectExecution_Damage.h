#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "NativeGameplayTags.h"
#include "GameplayEffectExecution_Damage.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SETBYCALLER_DAMAGE);

UCLASS()
class FIRSTPROJECT_API UGameplayEffectExecution_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGameplayEffectExecution_Damage();
	
protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};
