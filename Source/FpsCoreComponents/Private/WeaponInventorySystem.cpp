// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventorySystem.h"

// Sets default values for this component's properties
UWeaponInventorySystem::UWeaponInventorySystem()
{
	SetIsReplicatedByDefault(true);
}


void UWeaponInventorySystem::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	for(auto weaponptr: WeaponsRef)
	{
		weaponptr->Destroy();
	}
}

AWeaponBase* UWeaponInventorySystem::GetCurrentWeapon()
{
	if (CurrentWeaponIndex < WeaponsRef.Num())
	{
		return WeaponsRef[CurrentWeaponIndex];	
	}
	return nullptr;
}

void UWeaponInventorySystem::ChangeCurrentWeaponIndex(uint8 Slot)
{
	CurrentWeaponIndex = Slot;
	ChangeCurrentWeaponServer(Slot);
	OnWeaponChangeEvent.Broadcast(GetCurrentWeapon());
	OnAmmoChangeEvent.Broadcast(GetCurrentWeapon().)
}
void UWeaponInventorySystem::ChangeCurrentWeaponIndexServer_Implementation(uint8 Slot)
{
	CurrentWeaponIndex = Slot;
}

void UWeaponInventorySystem::AddWeaponToInventory_Implementation(AWeaponBase* NewWeapon)
{
	NewWeapon->SetOwner(GetOwner());
	WeaponsRef.Add(NewWeapon);
	OnInventoryChangedEvent.Broadcast();
	NewWeapon->OnAmmoChangedEvent.AddDynamic(this,&UWeaponInventorySystem::BroadcastAmmoChange);
	
}