#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputMappingContext.h"
#include "Abilities/AbilitySet.h"
#include "Attributes/StandardAttributeSet.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "FirstProjectCharacter.generated.h"

USTRUCT()
struct FAbilityInputToInputActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInput AbilityInput {EAbilityInput::None};
};

USTRUCT()
struct FAbilityInputBindings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TArray<FAbilityInputToInputActionBinding> Bindings;
};
UCLASS()
class FIRSTPROJECT_API AFirstProjectCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFirstProjectCharacter(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());
	
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;
	virtual void PawnClientRestart() override;
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	// Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()  const override { return AbilitySystemComponent; }
	// End IAbilitySystemInterface

	// Begin Attributes Helpers
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return GetHealth() > 0.f; }
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;	
	// End Attributes Helpers

	UFUNCTION(Exec)
	void CommandOutputCharacter();



protected:
	UPROPERTY(EditAnywhere, Category="Input|Binding")
	UInputMappingContext* InputMappingContext { nullptr };

	UPROPERTY(EditAnywhere, Category="Input|Binding")
	UInputAction* MovementAction { nullptr };
	
	UPROPERTY(EditAnywhere, Category="Input|Binding")
	UInputAction* JumpAction { nullptr };

	UPROPERTY(EditAnywhere, Category="Input|Binding")
	UInputAction* LookAction { nullptr };

	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UAbilitySystemComponent* AbilitySystemComponent { nullptr };

	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UStandardAttributeSet* StandardAttributes { nullptr };

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	UAbilitySet* InitialAbilitySet { nullptr };

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect; 

	UPROPERTY(EditDefaultsOnly, Category="Input|Binding")
	FAbilityInputBindings AbilityInputBindings;

	UPROPERTY(EditAnywhere)
	class UDemoActorComponent* DemoActorComponent { nullptr };

	// Begin Input Bindings
	void MovementInputHandler(const FInputActionValue& inputActionValue);
	void LookInputInputHandler(const FInputActionValue& inputActionValue);
	void AbilityInputBindingPressedHandler(EAbilityInput abilityInput);
	void AbilityInputBindingReleasedHandler(EAbilityInput abilityInput);
	// End Input Bindings
	
	void SetupInitialAbilitiesAndEffects();

	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;
	// Begin Attribute Bindings
	void OnHealthAttributeChanged(const FOnAttributeChangeData& onAttributeChangeData) const;
	// End Attribute Bindings

private:
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;
};
