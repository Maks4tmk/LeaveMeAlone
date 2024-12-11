// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"



ALMADefaultCharacter::ALMADefaultCharacter()
{

 	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetWorldRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}


void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial){
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector());
	}
	
}


void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		K2_SetActorRotation(FRotator(0.0f, FindRotatorResultYaw, 0.0f),true);
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}

}


void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAction("ZoomCamera_Out", IE_Pressed, this, &ALMADefaultCharacter::ZoomCamera_Out);
	PlayerInputComponent->BindAction("ZoomCamera_In", IE_Pressed, this, &ALMADefaultCharacter::ZoomCamera_In);
}


void ALMADefaultCharacter::MoveForward(float Value) {
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value) {
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::ZoomCamera_In(){
	SpringArmComponent->TargetArmLength = UKismetMathLibrary::FClamp(SpringArmComponent->TargetArmLength + ZoomStep, ZoomMin, ZoomMax);
}

void ALMADefaultCharacter::ZoomCamera_Out(){
	SpringArmComponent->TargetArmLength = UKismetMathLibrary::FClamp(SpringArmComponent->TargetArmLength - ZoomStep, ZoomMin, ZoomMax);
}