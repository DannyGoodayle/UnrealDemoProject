// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilitySet.h"

#include "AbilitySystemComponent.h"

TArray<FGameplayAbilitySpecHandle> UAbilitySet::GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const
{
	check(AbilitySystemComponent);

	TArray<FGameplayAbilitySpecHandle> handles;
	handles.Reserve(AbilitySetItems.Num());

	for (const auto abilitySetItem : AbilitySetItems)
	{
		handles.AddUnique(AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(
					abilitySetItem.GameplayAbility,
					0,
					static_cast<uint32>(abilitySetItem.InputKey)
				)
			));
	}

	return handles;
}
