// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "WeaponBase.h"

#include "WeaponInventorySystem.generated.h"

//Interface for hud to know what to render on the screen stats such as ammo.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange,int,CurrentAmmo,int,TotalAmmo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewWeapon,AWeaponBase *,NewWeapon);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSCORECOMPONENTS_API UWeaponInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponInventorySystem();
	void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	//handles the right click shoot button and shoots from the current weapon
	bool Shoot();

	//actually reload weapon. Careful to not confuse with the reloading animation.
	void Reload();

	//checks if current weapon is able to reload.
    bool CanReload();

	//handles DumpWeapon from inventory action
    void DumpWeapon();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	AWeaponBase *GetCurrentWeapon();
	
	void ChangeCurrentWeaponIndex(int Slot);
	
	UFUNCTION(Server,Reliable,BlueprintCallable)
    void RemoveWeaponFromInventory(int Slot);

	UFUNCTION(Server,Reliable,BlueprintCallable)
    void ReplaceWeaponToInventory(AWeaponBase *NewWeapon);

	//Interface for hud to know what to render on the screen stats such as ammo.
	UPROPERTY(BlueprintAssignable)
	FOnAmmoChange OnAmmoChangeEvent;

	//tells the owner that a weapon have been added, so it can attach it to socket
	UPROPERTY(BlueprintAssignable)
	FOnNewWeapon OnWeaponAdded;
	
	UPROPERTY(BlueprintAssignable)
	FOnNewWeapon OnWeaponRemoved;

	//Interface for Animation BP to switch between their modes. Broadcasted each time player presses 1,2,3,4 or reciev a new weapon
	UPROPERTY(BlueprintAssignable)
	FOnNewWeapon OnWeaponChanged;

	int GetCurrentSlot();

	

protected:
	
	UPROPERTY(ReplicatedUsing=RepNotifyWeaponListChanged)
	TArray<AWeaponBase *> WeaponsRef;

	//hackish way to check if a weapon have been removed or added.
	UPROPERTY()
	TArray<AWeaponBase *> WeaponsRefShadow;

	//treated as input therefore not replicated back to owner. Avoid using RPCs for replicating stuff such as inventory removing itens
	UPROPERTY(ReplicatedUsing=RepNotifyWeaponIndexChanged)
	int CurrentWeaponIndex;
	
	UFUNCTION()
    void RepNotifyWeaponIndexChanged();

	UFUNCTION()
    void RepNotifyWeaponListChanged();

	//multiyplayer stuff
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

private:
	void BroadcastAmmo();
	void UpdateVisibility();
	
	//called on server when scrolling from weapons in inventory
	UFUNCTION(Server,Reliable,BlueprintCallable)
    void ChangeCurrentWeaponIndexServer(int Slot);

	void SwitchToNextWeapon();
	
	
	
	
	
		
};
