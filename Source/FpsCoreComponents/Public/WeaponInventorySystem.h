// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "WeaponBase.h"

#include "WeaponInventorySystem.generated.h"

//tells the owner that a weapon have been added, so it can attach it to socket
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAddedRemoved,AWeaponBase *,NewWeapon);

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
	void Shoot();

	//actually reload weapon. Careful to not confuse with the reloading animation.
	void Reload();

	//actually reload weapon. Careful to not confuse with the reloading animation.
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

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChange OnAmmoChangeEvent;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponAddedRemoved OnWeaponAdded;
	
	UPROPERTY(BlueprintAssignable)
	FOnWeaponAddedRemoved OnWeaponRemoved;

	int GetCurrentSlot();

	

protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int MaxSlots=4;
	
	UPROPERTY(ReplicatedUsing=RepNotifyWeaponListChanged)
	TArray<AWeaponBase *> WeaponsRef;

	//hackish way to check if a weapon have been removed or added.
	UPROPERTY()
	TArray<AWeaponBase *> WeaponsRefShadow;

	//treated as input therefore not replicated back to owner. Avoid using RPCs for replicating sutuff such as inventory removing itens
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
	
	
	
	
	
		
};
