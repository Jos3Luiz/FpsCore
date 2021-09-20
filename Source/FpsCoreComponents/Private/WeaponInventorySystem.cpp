// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponInventorySystem.h"

#include "Math/UnrealMathUtility.h"
#include "AmmoTypes.h"

UWeaponInventorySystem::UWeaponInventorySystem()
{
	SetIsReplicatedByDefault(true);
	WeaponsRef.SetNum(EWeaponSlot::WS_Size);
	WeaponsRefShadow.SetNum(EWeaponSlot::WS_Size);
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

bool UWeaponInventorySystem::Shoot()
{
	AWeaponBase *CurrWeapon=GetCurrentWeapon();
	int CurrentAmmo;
	int StockAmmo;
	
	if(CurrWeapon)
	{
		CurrWeapon->GetAmmoStatus(CurrentAmmo,StockAmmo);
		if(CurrentAmmo>0)
		{
			CurrWeapon->Shoot(GetOwner());
			BroadcastAmmo();
			return true;			
		}
	}
	return false;
	
}

void UWeaponInventorySystem::Reload()
{
	AWeaponBase *CurrWeapon=GetCurrentWeapon();
	if(CurrWeapon)
	{
		if(CurrWeapon->CanReloadWeapon())
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
	OnWeaponChanged.Broadcast(GetCurrentWeapon());
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
	if (CurrentWeaponIndex < WeaponsRef.Num() && Slot!=CurrentWeaponIndex)
	{
		if(WeaponsRef[Slot]!=nullptr)
		{
			CurrentWeaponIndex = Slot;
			BroadcastAmmo();
			OnWeaponChanged.Broadcast(GetCurrentWeapon());
			UpdateVisibility();
			ChangeCurrentWeaponIndexServer(Slot);			
		}

	}
}

void UWeaponInventorySystem::RemoveWeaponFromInventory_Implementation(int Slot)
{
	if(Slot < WeaponsRef.Num())
	{
		OnWeaponRemoved.Broadcast(WeaponsRef[Slot]);
		WeaponsRef[Slot] = nullptr;
		OnWeaponChanged.Broadcast(GetCurrentWeapon());
	
	}
	
}

void UWeaponInventorySystem::ReplaceWeaponToInventory_Implementation(AWeaponBase* NewWeapon)
{
	AWeaponBase *CurrWeapon = GetCurrentWeapon();
	if (NewWeapon)
	{
		if(CurrWeapon)
		{
			if(CurrWeapon->Slot==NewWeapon->Slot)
			{
				CurrWeapon->SetOwner(nullptr);
				OnWeaponRemoved.Broadcast(CurrWeapon);				
			}

		}
		NewWeapon->SetOwner(GetOwner());
		WeaponsRef[NewWeapon->Slot] = NewWeapon;
		OnWeaponAdded.Broadcast(NewWeapon);
		OnWeaponChanged.Broadcast(GetCurrentWeapon());
		BroadcastAmmo();
		ChangeCurrentWeaponIndex(NewWeapon->Slot);
	}

}


void UWeaponInventorySystem::ChangeCurrentWeaponIndexServer_Implementation(int Slot)
{
	if (CurrentWeaponIndex < WeaponsRef.Num())
	{
		CurrentWeaponIndex = Slot;
		RepNotifyWeaponIndexChanged();
	}
}

void UWeaponInventorySystem::RepNotifyWeaponListChanged()
{
	for(int i=0;i<WeaponsRef.Num();i++)
	{
		if(WeaponsRefShadow[i]!=WeaponsRef[i])
		{
			if(WeaponsRefShadow[i])
			{
				OnWeaponRemoved.Broadcast(WeaponsRefShadow[i]);
				OnWeaponChanged.Broadcast(GetCurrentWeapon());
			}
			if(WeaponsRef[i])
			{
				OnWeaponAdded.Broadcast(WeaponsRef[i]);
				OnWeaponChanged.Broadcast(GetCurrentWeapon());
				ChangeCurrentWeaponIndex(WeaponsRef[i]->Slot);
				
			}
		}
		WeaponsRefShadow[i] = WeaponsRef[i];
	}
	
	RepNotifyWeaponIndexChanged();
	BroadcastAmmo();
}


void UWeaponInventorySystem::RepNotifyWeaponIndexChanged()
{
	BroadcastAmmo();
	OnWeaponChanged.Broadcast(GetCurrentWeapon());
	UpdateVisibility();
}

void UWeaponInventorySystem::BroadcastAmmo()
{
	int CurrAmmo,StockAmmo;
	SwitchToNextWeapon();
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
	BroadcastAmmo();
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

void UWeaponInventorySystem::SwitchToNextWeapon()
{
	int i;
	for(i=CurrentWeaponIndex; i<WeaponsRef.Num();i++)
	{
		if (!WeaponsRef[i])
		{
			ChangeCurrentWeaponIndex(i);
		}
	}
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
