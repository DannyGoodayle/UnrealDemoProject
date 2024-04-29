
#include "Character/FirstProjectCharacter.h"
#include "Character/Attributes/StandardAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/DemoActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"

static TAutoConsoleVariable<int32> CVarCommandOutput(
	TEXT("demo.CommandOutput"),
	0,
	TEXT("Prints this number to console when you type CommandOutputCharacter"));

AFirstProjectCharacter::AFirstProjectCharacter(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	StandardAttributes = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributeSet"));
	DemoActorComponent = CreateDefaultSubobject<UDemoActorComponent>(TEXT("DemoComp"));
}

void AFirstProjectCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);
	if (UEnhancedInputComponent* playerEnhancedInputComponent = Cast<UEnhancedInputComponent>(playerInputComponent))
	{
		if (MovementAction)
		{
			playerEnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ThisClass::MovementInputHandler);
		}
		if (LookAction)
		{
			playerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInputInputHandler);
		}
		for (const FAbilityInputToInputActionBinding& binding : AbilityInputBindings.Bindings)
		{
			playerEnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Triggered, this, &ThisClass::AbilityInputBindingPressedHandler, binding.AbilityInput);
			playerEnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Completed, this, &ThisClass::AbilityInputBindingReleasedHandler, binding.AbilityInput);
		}
	}
}

void AFirstProjectCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	if (const APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->ClearAllMappings();
			subsystem->AddMappingContext(InputMappingContext, 1);
		}
	}
}

void AFirstProjectCharacter::SetupInitialAbilitiesAndEffects()
{
	if(IsValid(AbilitySystemComponent) == false || IsValid(StandardAttributes) == false)
	{
		return;
	}
	
	if(IsValid(InitialAbilitySet))
	{
		InitiallyGrantedAbilitySpecHandles.Append(InitialAbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
	}
	
	if(IsValid(InitialGameplayEffect))
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(), 0.f, AbilitySystemComponent->MakeEffectContext());
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStandardAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
}

bool AFirstProjectCharacter::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	if(DemoActorComponent && DemoActorComponent->ProcessConsoleExec(Cmd, Ar, Executor))
	{
		return true;
	}
	return Super::ProcessConsoleExec(Cmd, Ar, Executor);
}

void AFirstProjectCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& onAttributeChangeData) const
{
	if(FMath::IsNearlyEqual(onAttributeChangeData.NewValue, 0.f) && onAttributeChangeData.OldValue > 0)
	{
		// our health has just reached zero, we need to apply a "death" effect to the character
		
	}
}

void AFirstProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		SetupInitialAbilitiesAndEffects();
	}
}

void AFirstProjectCharacter::BeginDestroy()
{
	Super::BeginDestroy();
}

// Begin Attributes Helpers
float AFirstProjectCharacter::GetHealth() const
{
	if(IsValid(StandardAttributes) == false)
	{
		return 0.f;
	}
	return StandardAttributes->GetHealth();
}
// End Attribute Helpers

void AFirstProjectCharacter::CommandOutputCharacter()
{
	UE_LOG(LogTemp, Log, TEXT("The value was set to %i"), CVarCommandOutput.GetValueOnGameThread());	
}


void AFirstProjectCharacter::MovementInputHandler(const FInputActionValue& inputActionValue)
{
	const FVector valueAsVector = inputActionValue.Get<FVector>();
	AddMovementInput(GetActorForwardVector(), valueAsVector.Y);
	AddMovementInput(GetActorRightVector(), valueAsVector.X);
}

void AFirstProjectCharacter::LookInputInputHandler(const FInputActionValue& inputActionValue)
{
	const FVector valueAsVector = inputActionValue.Get<FVector>();
	AddControllerYawInput(valueAsVector.X);
	AddControllerPitchInput(valueAsVector.Y);
}

void AFirstProjectCharacter::AbilityInputBindingPressedHandler(EAbilityInput abilityInput)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(abilityInput));
}


void AFirstProjectCharacter::AbilityInputBindingReleasedHandler(EAbilityInput abilityInput)
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<uint32>(abilityInput));
}
