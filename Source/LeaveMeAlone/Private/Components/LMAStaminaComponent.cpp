// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Components/LMAStaminaComponent.h"

ULMAStaminaComponent::ULMAStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULMAStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentStamina = MaxStamina;
	OnStaminaChanged.Broadcast(CurrentStamina);
}

void ULMAStaminaComponent::ConsumeStamina() {
	CurrentStamina = FMath::Clamp(CurrentStamina - StaminaUsageValue, 0.0f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina);
}

void ULMAStaminaComponent::RegenerateStamina() {
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRefillValue, 0.0f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina);
}


