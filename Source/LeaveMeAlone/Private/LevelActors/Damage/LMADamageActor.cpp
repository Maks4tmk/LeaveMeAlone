// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LevelActors/Damage/LMADamageActor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ALMADamageActor::ALMADamageActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetSphereRadius(SphereRadius);
	SetRootComponent(SphereComponent);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	ParticleSystemComponent->SetupAttachment(SphereComponent);
	ParticleSystemComponent->SetRelativeScale3D(FVector(2.5f, 2.5f, 0.0f));
	

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SphereComponent);
	StaticMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.05f));
	StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));

}

void ALMADamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALMADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), SphereRadius, nullptr, {}, this, nullptr, false);
}

