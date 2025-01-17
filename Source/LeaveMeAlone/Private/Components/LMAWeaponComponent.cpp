// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}


void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void ULMAWeaponComponent::StartFire() {
	if (Weapon && !AnimReloading)
	{
		Weapon->StartFire();
	}
}

void ULMAWeaponComponent::StopFire()
{
	if (Weapon && !AnimReloading)
	{
		Weapon->StopFire();
	}
}

void ULMAWeaponComponent::SpawnWeapon() {
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		Weapon->OnClipEmpty.AddUObject(this, &ULMAWeaponComponent::OnClipEmpty);
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character){
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");
		}
	}
}

void ULMAWeaponComponent::InitAnimNotify() {
	if (!ReloadMontage)
		return;
	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh) {
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && Weapon->CanReload();
}

void ULMAWeaponComponent::Reload()
{
	ChangeClip();
}

void ULMAWeaponComponent::OnClipEmpty() {
	ChangeClip();
}

void ULMAWeaponComponent::ChangeClip() {
	if (!CanReload())
		return;
	Weapon->StopFire();
	Weapon->ChangeClip();
	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Weapon Reload"));
}



