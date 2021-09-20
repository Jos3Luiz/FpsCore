// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableBase.h"
#include "BulletDamageType.h"
#include "AmmoTypes.h"
#include "WeaponBase.generated.h"

UCLASS(Abstract)
class FPSCOREWEAPONS_API AWeaponBase : public ACollectableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	//called when player clicks, takes care of weapon cooldown
	virtual void Shoot(AActor* AInstigator);

	//when player reloads weapon
	virtual void ReloadWeapon()
	{
	}

	virtual bool CanReloadWeapon();

	FName GetSocketName();

	//Requests to change visibility for the weapon
	virtual void SetWeaponVisibility(bool Val);

	bool IsOnCooldown();

	void GetAmmoStatus(int& CurrentAmmoOut, int& CurrentStockAmmoOut);

	UFUNCTION(BlueprintPure)
	void GetAnimationConfigs(FName& SocketNameOut, FName& IdleMontageSectionNameOut, FName& ReloadMontageSectionNameOut,
	                         float& AimTimeOut);

	virtual void ChangeOutline(bool Val) override;

	virtual void OnRemoveFromInventory() override;

	virtual void OnAddToInventory() override;

	//Slot used on inventory
	UPROPERTY(EditDefaultsOnly, Category="WeaponProperties")
	TEnumAsByte<EWeaponSlot> Slot;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, Category="WeaponProperties")
	float FireTime = 0.0016f; //600rpm

	UPROPERTY(EditDefaultsOnly, Category="WeaponProperties")
	float WeaponDamage = 10;

	UPROPERTY(EditAnywhere, Category="WeaponProperties")
	float MaxEffectiveRange = 100000;

	//maximum ammo on the clip
	UPROPERTY(EditDefaultsOnly, Category="WeaponProperties")
	int MaxAmmo = 1;

	//current ammo on the clip
	UPROPERTY(EditDefaultsOnly, Category="WeaponProperties")
	int CurrentAmmo = 1;

	//maximum Ammo the weapon can hold
	UPROPERTY(EditDefaultsOnly, Category="WeaponProperties")
	int MaxStockAmmo = 1;

	//current ammo in the "inventory"
	UPROPERTY(EditDefaultsOnly, Category="WeaponProperties")
	int CurrentStockAmmo = 1;



	UPROPERTY(EditDefaultsOnly, Category="WeaponProperties")
	TSubclassOf<UDamageType> WeaponDamageType = UBulletDamageType::StaticClass();

	//impulse added to the character when receiving damage
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="WeaponProperties")
	float HitImpulseAmount = 400.0f;


	//socket expected to be used on the character. Change to melee socket if needed for example. The default is the base class name e.g. Firearm
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Animation")
	FName SocketName;

	//socket expected to be used on the character.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Animation")
	FName IdleMontageSectionName;

	//socket expected to be used on the character.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Animation")
	FName ReloadMontageSectionName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Animation")
	float AimTime = 0.1f;

	//Use it to spawn particles on the client side
	virtual void VisualShootLogic(AActor* AInstigator, FVector Start, FVector Direction);

	//use it to deal damage on the server side
	virtual void ServerShootLogic(AActor* AInstigator, FVector Start, FVector Direction);

	UFUNCTION(Server, Reliable)
	void ServerSideShoot(AActor* AInstigator, FVector Start, FVector Direction);

	UFUNCTION(NetMulticast, Unreliable)
	void ClientSideShoot(AActor* AInstigator, FVector Start, FVector Direction);

	//always true
	virtual bool IsSupportedForNetworking() const override;

private:

	void ResetCooldown();
	bool IsCooldown = false;
};
