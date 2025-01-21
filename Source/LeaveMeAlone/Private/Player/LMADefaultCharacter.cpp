// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Components/LMAHealthComponent.h"
#include "Components/LMAStaminaComponent.h"
#include "Components/LMAWeaponComponent.h"



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
	
	StaminaComponent = CreateDefaultSubobject<ULMAStaminaComponent>("StaminaComponent");

	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");

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

	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
	
}


void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!(HealthComponent->IsDead())){
		RotationPlayerOnCursor();
	}

	IsSprint = GetCharacterMovement()->IsExceedingMaxSpeed(MoveSpeed);

	if (IsSprint && StaminaComponent->GetStamina() != 0)
	{
		StaminaComponent->ConsumeStamina();
	}
	else if (!IsSprint && StaminaComponent->GetStamina() != StaminaComponent->GetMaxStamina())
	{
		StaminaComponent->RegenerateStamina();
	}

	if (FMath::IsNearlyZero(StaminaComponent->GetStamina()))
	{
		EndSprint();
	}

}


void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);

	PlayerInputComponent->BindAction("ZoomCamera_Out", IE_Pressed, this, &ALMADefaultCharacter::ZoomCamera_Out);
	PlayerInputComponent->BindAction("ZoomCamera_In", IE_Pressed, this, &ALMADefaultCharacter::ZoomCamera_In);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::EndSprint);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
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

void ALMADefaultCharacter::StartSprint() {
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}

void ALMADefaultCharacter::EndSprint() {
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void ALMADefaultCharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMADefaultCharacter::RotationPlayerOnCursor()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		K2_SetActorRotation(FRotator(0.0f, FindRotatorResultYaw, 0.0f), true);
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}


