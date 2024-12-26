// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMADamageActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS()
class LEAVEMEALONE_API ALMADamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALMADamageActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float SphereRadius = 100.0f;


protected:
	virtual void BeginPlay() override;

public:	

};
