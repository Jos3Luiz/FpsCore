// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	SocketName = TEXT("weaponSocket");
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SetRootComponent(Weapon);
	bReplicates=true;
	//Weapon->CastShadow(false);
	FireTime = 1.0f/(RoundsPerMinute/60.0f);

}

void AWeaponBase::Shoot(AActor* AInstigator)
{
	FTimerHandle UnusedHandle;
	IsCooldown=true;
	GetWorldTimerManager().SetTimer(UnusedHandle,this,&AWeaponBase::ResetCooldown,FireTime,false);
}

bool AWeaponBase::CanReloadWeapon()
{
	return false;
}


void AWeaponBase::ClientSideShoot_Implementation(AActor* AInstigator, FVector Start, FVector Direction)
{
	APawn *owner=Cast<APawn>(GetOwner());
	if(owner)
	{
		if (! owner->IsLocallyControlled())
		{
			VisualShootLogic(AInstigator,Start,Direction);		
		}
	}
	
}

void AWeaponBase::ServerSideShoot_Implementation(AActor* AInstigator, FVector Start, FVector Direction)
{
	ServerShootLogic(AInstigator,Start,Direction);
	ClientSideShoot(AInstigator,Start,Direction);
}

void AWeaponBase::SetWeaponVisibility(bool Val)
{
	Weapon->SetVisibility(Val);
}

FName AWeaponBase::GetSocketName()
{
	return SocketName;
}

void AWeaponBase::ResetCooldown()
{
	IsCooldown=false;
}

bool AWeaponBase::IsOnCooldown()
{
	return IsCooldown;
}

void AWeaponBase::GetAmmoStatus(int& CurrentAmmoOut, int& CurrentStockAmmoOut)
{
	CurrentAmmoOut=CurrentAmmo;
	CurrentStockAmmoOut=CurrentStockAmmo;
}

// for UE4 Multiplayer
bool AWeaponBase::IsSupportedForNetworking() const
{
	return true;
}




