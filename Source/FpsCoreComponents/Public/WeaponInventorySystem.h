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

	UFUNCTION(BlueprintCallable,BlueprintPure)
    AWeaponBase *GetCurrentWeapon();
	
	UFUNCTION(BlueprintCallable)
	void ChangeCurrentWeaponIndex(uint8 Slot);
	
	UFUNCTION(Server,Reliable,BlueprintCallable)
    void AddWeaponToInventory(AWeaponBase *NewWeapon);

	UFUNCTION(Server,Reliable,BlueprintCallable)
    void ReplaceWeaponToInventory(AWeaponBase *NewWeapon);

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChange OnAmmoChangeEvent;

	

protected:


	//called when scrolling from inputs
	UFUNCTION(Server,Reliable,BlueprintCallable)
    void ChangeCurrentWeaponIndexServer(uint8 Slot);
	
	UPROPERTY(ReplicatedUsing=RepNotifyInventoryChanged)
	TArray<AWeaponBase *> WeaponsRef;
	UFUNCTION()
    void RepNotifyInventoryChanged();

	//treated as input therefore not replicated back to owner
	UPROPERTY(ReplicatedUsing=RepNotifyWeaponIndexChanged)
	uint8 CurrentWeaponIndex;
	UFUNCTION()
    void RepNotifyWeaponIndexChanged();

	//broadcasts the last level OnAmmoChangeEvent, implementing a delegate for the whole inventory ammo change.
	void BroadcastAmmoChange(int CurrentAmmo,int CurrentStockAmmo);

	
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	
	
	
		
};
