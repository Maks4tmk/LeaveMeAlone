// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct  FAmmoWeapon{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};


UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ALMABaseWeapon();
	virtual void Tick(float DeltaTime) override;

	FOnClipEmptySignature OnClipEmpty;

	FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }

	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	void ChangeClip();
	bool CanReload();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon{30, 0, true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShots = 0.1f;

	void Shot();

	void DecrementBullets();
	bool IsCurrentClipEmpty() const;


private:
	FAmmoWeapon CurrentAmmoWeapon;

	FTimerHandle ShotTimerHandle;
};
