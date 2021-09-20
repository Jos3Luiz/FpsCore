// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "BulletDamageType.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	SocketName = TEXT("weaponSocket");
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SetRootComponent(Weapon);
	bReplicates=true;
	//Weapon->CastShadow(false);
	Weapon->SetSimulatePhysics(true);
	

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

void AWeaponBase::ChangeOutline(bool Val)
{
	Weapon->SetRenderCustomDepth(Val);
}

void AWeaponBase::OnAddToInventory()
{
	Weapon->SetSimulatePhysics(false);
	SetActorEnableCollision(false);
}

void AWeaponBase::OnRemoveFromInventory()
{
	Weapon->SetSimulatePhysics(true);
	SetActorEnableCollision(true);
}

void AWeaponBase::ServerShootLogic(AActor* AInstigator, FVector Start, FVector Direction)
{
	FHitResult HitDetails = FHitResult(ForceInit);
	FDamageEvent DamageEvent;
	
	FCollisionQueryParams TraceParams(FName(TEXT("DamageTrace")), false, NULL);
	TraceParams.bReturnPhysicalMaterial = true;
	
	bool HitSuccess = GetWorld()->LineTraceSingleByChannel(HitDetails,Start,Start+Direction*MaxEffectiveRange,ECollisionChannel::ECC_Visibility,TraceParams);
	
	//DrawDebugLine(GetWorld(), start, start+direction*10000, FColor::Red, false, 5.f, ECC_WorldStatic, 1.f);
	if (HitSuccess)
	{
		UGameplayStatics::ApplyPointDamage(HitDetails.Actor.Get(),WeaponDamage,Start,HitDetails,GetOwner()->GetInstigatorController(),this,WeaponDamageType);	
	}
}


void AWeaponBase::VisualShootLogic(AActor* AInstigator, FVector Start, FVector Direction)
{

	FHitResult HitDetails = FHitResult(ForceInit);
	FCollisionQueryParams TraceParams(FName(TEXT("DamageTrace")), false, NULL);
	TraceParams.bReturnPhysicalMaterial = true;

	bool HitSuccess = GetWorld()->LineTraceSingleByChannel(HitDetails,Start,Start+Direction*MaxEffectiveRange,ECollisionChannel::ECC_Visibility,TraceParams);
	if (HitSuccess)
	{
		//ACharacter *DamagedActor = Cast<ACharacter>(HitDetails.Actor);
		//spawn damage particles here
	}
	
}

void AWeaponBase::GetAnimationConfigs(FName& SocketNameOut, FName& IdleMontageSectionNameOut, FName& ReloadMontageSectionNameOut, float& AimTimeOut)
{
	SocketNameOut=SocketName;
	IdleMontageSectionNameOut=IdleMontageSectionName;
	ReloadMontageSectionNameOut=ReloadMontageSectionName;
	AimTimeOut=AimTime;
	
}

