// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "WeaponBase.h"

#include "WeaponInventorySystem.generated.h"

//Interface for hud to know what to render on the screen stats such as ammo.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange,int,CurrentAmmo,int,TotalAmmo);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSCORECOMPONENTS_API UWeaponInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponInventorySystem();
	void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	//handles the right click shoot button and shoots from the current weapon
	UFUNCTION(BlueprintCallable)
	void Shoot();

	//actually reload weapon. Careful to not confuse with the reloading animation.
	UFUNCTION(BlueprintCallable)
	void Reload();

	//actually reload weapon. Careful to not confuse with the reloading animation.
	UFUNCTION(BlueprintCallable)
    bool CanReload();

	//handles DumpWeapon from inventory action
	UFUNCTION(BlueprintCallable)
    void DumpWeapon();
	
	UFUNCTION(BlueprintCallable)
	void ChangeCurrentWeaponIndex(int Slot);
	
	UFUNCTION(Server,Reliable,BlueprintCallable)
    void AddWeaponToInventory(AWeaponBase *NewWeapon);

	UFUNCTION(Server,Reliable,BlueprintCallable)
    void RemoveWeaponFromInventory(int Slot);

	UFUNCTION(Server,Reliable,BlueprintCallable)
    void ReplaceWeaponToInventory(AWeaponBase *NewWeapon);

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChange OnAmmoChangeEvent;

	

protected:

	
	UPROPERTY(ReplicatedUsing=RepNotifyWeaponIndexChanged)
	TArray<AWeaponBase *> WeaponsRef;

	//treated as input therefore not replicated back to owner
	UPROPERTY(ReplicatedUsing=RepNotifyWeaponIndexChanged)
	int CurrentWeaponIndex;
	
	UFUNCTION()
    void RepNotifyWeaponIndexChanged();

	//multiyplayer stuff
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

private:
	void BroadcastAmmo();
	void UpdateVisibility();
	
	//called on server when scrolling from weapons in inventory
	UFUNCTION(Server,Reliable,BlueprintCallable)
    void ChangeCurrentWeaponIndexServer(int Slot);
	
	AWeaponBase *GetCurrentWeapon();
	
	
	
		
};
