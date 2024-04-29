// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attributes/StandardAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Character/FirstProjectCharacter.h"
#include "GameFramework/FirstProjectPlayerController.h"
#include "Net/UnrealNetwork.h"

void UStandardAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetHealthMax());
	}
}

void UStandardAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			// Apply the health change and then clamp it
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetHealthMax()));
		}
	}// Damage
}


void UStandardAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStandardAttributeSet, Health, OldHealth);
}

void UStandardAttributeSet::OnRep_HealthMax(const FGameplayAttributeData& OldHealthMax)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStandardAttributeSet, HealthMax, OldHealthMax);
}

void UStandardAttributeSet::OnRep_DamageIncomingMultiplier(const FGameplayAttributeData& OldDamageIncomingMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStandardAttributeSet, DamageIncomingMultiplier, OldDamageIncomingMultiplier);

}

void UStandardAttributeSet::OnRep_DamageOutgoingMultiplier(const FGameplayAttributeData& OldDamageOutgoingMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStandardAttributeSet, DamageOutgoingMultiplier, OldDamageOutgoingMultiplier);
}

void UStandardAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UStandardAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UStandardAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UStandardAttributeSet, DamageIncomingMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UStandardAttributeSet, DamageOutgoingMultiplier, COND_None, REPNOTIFY_Always);
}
