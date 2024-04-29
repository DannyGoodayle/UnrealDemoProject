// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityInputs.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "AbilitySet.generated.h"


USTRUCT()
struct FAbilitySetItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	EAbilityInput InputKey {EAbilityInput::None};
};

UCLASS()
class FIRSTPROJECT_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TArray<FAbilitySetItem> AbilitySetItems;

	TArray<FGameplayAbilitySpecHandle> GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const;
};
