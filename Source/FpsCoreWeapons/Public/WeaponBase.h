// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "InteractableBase.h"
#include "WeaponBase.generated.h"

UCLASS(Abstract)
class FPSCOREWEAPONS_API AWeaponBase : public AInteractableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	//called when player clicks, takes care of weapon cooldown
    virtual void Shoot(AActor* AInstigator);
	
	//when player reloads weapon
    virtual void ReloadWeapon(){}

	virtual bool CanReloadWeapon();

	FName GetSocketName();

	//Requests to change visibility for the weapon
	virtual void SetWeaponVisibility(bool Val);

	bool IsOnCooldown();

	void GetAmmoStatus(int &CurrentAmmoOut,int &CurrentStockAmmoOut);
	
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent *Weapon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RoundsPerMinute=1200;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float WeaponDamage=10;

	//maximum ammo on the clip
	UPROPERTY(EditAnywhere)
	int MaxAmmo=1;

	//current ammo on the clip
	UPROPERTY(EditAnywhere)
	int CurrentAmmo=1;

	//maximum Ammo the weapon can hold
	UPROPERTY(EditAnywhere)
	int MaxStockAmmo=1;

	//current ammo in the "inventory"
	UPROPERTY(EditAnywhere)
	int CurrentStockAmmo=1;

	//socket expected to be used on the character.WeaponSocket by default. Change to melee socket if needed for example.
	UPROPERTY(EditAnywhere)
	FName SocketName;


	
	//Use it to spawn particles on the client side
	virtual void VisualShootLogic(AActor *AInstigator,FVector Start,FVector Direction)
	PURE_VIRTUAL(AWeaponBase::VisualShootShoot);

	//use it to deal damage on the server side
	virtual void ServerShootLogic(AActor *AInstigator,FVector Start,FVector Direction)
	PURE_VIRTUAL(AWeaponBase::ServerShootLogic);

	UFUNCTION(Server,Reliable)
	void ServerSideShoot(AActor *AInstigator,FVector Start,FVector Direction);

	UFUNCTION(NetMulticast,Unreliable)
    void ClientSideShoot(AActor *AInstigator,FVector Start,FVector Direction);

	//always true
	virtual bool IsSupportedForNetworking() const override;
	
private:

	void ResetCooldown();
	
	float FireTime;

	bool IsCooldown=false;




};
