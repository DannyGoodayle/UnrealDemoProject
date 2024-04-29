// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeHelpers.h"
#include "AbilitySystemComponent.h"
#include "StandardAttributeSet.generated.h"

UCLASS()
class FIRSTPROJECT_API UStandardAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_HealthMax)
	FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, HealthMax);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_DamageIncomingMultiplier)
	FGameplayAttributeData DamageIncomingMultiplier;
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, DamageIncomingMultiplier);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_DamageOutgoingMultiplier)
	FGameplayAttributeData DamageOutgoingMultiplier;
	
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, DamageOutgoingMultiplier);

	// meta attribute - server-side only
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UStandardAttributeSet, Damage)


	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_HealthMax(const FGameplayAttributeData& OldHealthMax);
	
	UFUNCTION()
	void OnRep_DamageIncomingMultiplier(const FGameplayAttributeData& OldDamageIncomingMultiplier);
	
	UFUNCTION()
	void OnRep_DamageOutgoingMultiplier(const FGameplayAttributeData& OldDamageOutgoingMultiplier);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
