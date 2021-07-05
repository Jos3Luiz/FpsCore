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
	OnAmmoChangedEvent.Broadcast(CurrentAmmo,CurrentStockAmmo);
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

void AWeaponBase::SetWeaponVisibilityCallback(bool Val)
{
	Weapon->SetVisibility(Val);
}

FName AWeaponBase::GetSocketName()
{
	return SocketName;
}

bool AWeaponBase::CanReload()
{
	return CurrentAmmo < MaxAmmo && CurrentStockAmmo > 0;
}

/*
void AWeaponBase::ChangeWeaponVisibility(bool Val)
{
	SetWeaponVisibilityCallback(Val);
	ServerSideSetWeaponVisibility(Val);
}

void AWeaponBase::RepNotifyOnVisibilityChanged(bool Val)
{
	SetWeaponVisibilityCallback(Val);
}

void AWeaponBase::ServerSideSetWeaponVisibility_Implementation(bool Val)
{
	WeaponVisibility=Val;
}
*/



void AWeaponBase::ResetCooldown()
{
	IsCooldown=false;
}

bool AWeaponBase::IsOnCooldown()
{
	return IsCooldown;
}







// for UE4 Multiplayer
/*
 *Deprecated. Weapon Inventory deals locally with that
 *void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION( AWeaponBase, WeaponVisibility, COND_SkipOwner );
}
*/

bool AWeaponBase::IsSupportedForNetworking() const
{
	return true;
}




