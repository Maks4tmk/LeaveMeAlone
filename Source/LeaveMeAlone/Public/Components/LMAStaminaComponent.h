// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAStaminaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULMAStaminaComponent();

	UFUNCTION(BlueprintCallable)
	float GetStamina() const { return CurrentStamina; }
	float GetMaxStamina() const { return MaxStamina; }

	void ConsumeStamina();
	void RegenerateStamina();

	FOnStaminaChanged OnStaminaChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float StaminaRefillValue = 0.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float StaminaUsageValue = 0.5f;

private:	
	float CurrentStamina = 0.0f;
		
};
