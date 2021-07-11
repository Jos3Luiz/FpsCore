// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponInventorySystem.h"

#include "Math/UnrealMathUtility.h"


UWeaponInventorySystem::UWeaponInventorySystem()
{
	SetIsReplicatedByDefault(true);
	WeaponsRef.SetNum(MaxSlots);
	WeaponsRefShadow.SetNum(MaxSlots);
}



void UWeaponInventorySystem::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	for(auto weaponptr: WeaponsRef)
	{
		if (weaponptr)
		{
			weaponptr->Destroy();	
		}
		
	}
}

void UWeaponInventorySystem::Shoot()
{
	AWeaponBase *CurrWeapon=GetCurrentWeapon();
	if(CurrWeapon)
	{
		CurrWeapon->Shoot(GetOwner());
	}
	BroadcastAmmo();
}

void UWeaponInventorySystem::Reload()
{
	AWeaponBase *CurrWeapon=GetCurrentWeapon();
	if(CurrWeapon)
	{
		if(!CurrWeapon->CanReloadWeapon())
		{
			CurrWeapon->ReloadWeapon();
		}
	}
	BroadcastAmmo();

}

bool UWeaponInventorySystem::CanReload()
{
	AWeaponBase *CurrWeapon=GetCurrentWeapon();
	if(CurrWeapon)
	{
		return CurrWeapon->CanReloadWeapon();
	}
	return false;
}

void UWeaponInventorySystem::DumpWeapon()
{
	
	ChangeCurrentWeaponIndex(FMath::Clamp(CurrentWeaponIndex+1,0, WeaponsRef.Num()-1));
	RemoveWeaponFromInventory(CurrentWeaponIndex);
	UpdateVisibility();
	BroadcastAmmo();
}


AWeaponBase* UWeaponInventorySystem::GetCurrentWeapon()
{
	if (CurrentWeaponIndex < WeaponsRef.Num())
	{
		return WeaponsRef[CurrentWeaponIndex];	
	}
	return nullptr;
}

void UWeaponInventorySystem::ChangeCurrentWeaponIndex(int Slot)
{
	if (CurrentWeaponIndex < WeaponsRef.Num())
	{
		CurrentWeaponIndex = Slot;
		BroadcastAmmo();
		UpdateVisibility();
		ChangeCurrentWeaponIndexServer(Slot);
	}
}

void UWeaponInventorySystem::RemoveWeaponFromInventory_Implementation(int Slot)
{
	if(Slot < WeaponsRef.Num())
	{
		OnWeaponRemoved.Broadcast(WeaponsRef[Slot]);
		WeaponsRef[Slot] = nullptr;
	}
	
}

void UWeaponInventorySystem::ReplaceWeaponToInventory_Implementation(AWeaponBase* NewWeapon)
{
	AWeaponBase *CurrWeapon = GetCurrentWeapon();
	if (NewWeapon)
	{
		if(CurrWeapon)
		{
			CurrWeapon->SetOwner(nullptr);
			OnWeaponRemoved.Broadcast(CurrWeapon);
		}
		NewWeapon->SetOwner(GetOwner());
		WeaponsRef[CurrentWeaponIndex] = NewWeapon;
		OnWeaponAdded.Broadcast(NewWeapon);
	}

}


void UWeaponInventorySystem::ChangeCurrentWeaponIndexServer_Implementation(int Slot)
{
	if (CurrentWeaponIndex < WeaponsRef.Num())
	{
		CurrentWeaponIndex = Slot;
	}
}

void UWeaponInventorySystem::RepNotifyWeaponListChanged()
{
	for(int i=0;i<MaxSlots;i++)
	{
		if(WeaponsRefShadow[i]!=WeaponsRef[i])
		{
			if(WeaponsRefShadow[i])
			{
				OnWeaponRemoved.Broadcast(WeaponsRefShadow[i]);
			}
			if(WeaponsRef[i])
			{
				OnWeaponAdded.Broadcast(WeaponsRef[i]);
			}
		}
		WeaponsRefShadow[i] = WeaponsRef[i];
	}
	RepNotifyWeaponIndexChanged();
}


void UWeaponInventorySystem::RepNotifyWeaponIndexChanged()
{
	BroadcastAmmo();
	UpdateVisibility();
}

void UWeaponInventorySystem::BroadcastAmmo()
{
	int CurrAmmo,StockAmmo;
	AWeaponBase *CurrWeapon= GetCurrentWeapon();
	if(CurrWeapon)
	{
		CurrWeapon->GetAmmoStatus(CurrAmmo,StockAmmo);
		OnAmmoChangeEvent.Broadcast(CurrAmmo,StockAmmo);
	}
}
void UWeaponInventorySystem::UpdateVisibility()
{
	AWeaponBase *CurrWeapon= GetCurrentWeapon();
	for (AWeaponBase *i : WeaponsRef)
	{
		if(i) //if valid ptr
		{
			if(i==CurrWeapon)
			{
				i->SetWeaponVisibility(true);
			}
			else
			{
				i->SetWeaponVisibility(false);
			}
		}
	}
}

int UWeaponInventorySystem::GetCurrentSlot()
{
	return CurrentWeaponIndex;
}


//------------------------------ for replication
bool UWeaponInventorySystem::IsSupportedForNetworking() const
{	
	return true;	
}

void UWeaponInventorySystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponInventorySystem, WeaponsRef);
	DOREPLIFETIME_CONDITION( UWeaponInventorySystem, CurrentWeaponIndex, COND_SkipOwner );
}
