// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthSystem.h"
#include "WeaponInventorySystem.h"
#include "InventorySystem.h"
#include "AnimationEnum.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "PlayerBase.generated.h"

//interface for the anim BP know what current animation is playing, therefore no BP dependency is needed.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimChange,EAnimEnumPose,AnimationPoseIndex);

//interface for the anim BP know what current short animation is playing, this one is called by RPCs such as shoot or reload.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimTransientChange,EAnimEnumTransient,AnimationPoseIndex);


UCLASS(Abstract)
class FPSCORECHARACTERS_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerBase();

	UFUNCTION(BlueprintCallable)
    void SetCanCharacterMove(const bool Val);
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
    FORCEINLINE bool GetCanCharacterMove();
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
    FORCEINLINE UWeaponInventorySystem *GetPlayerWeaponInventory();

	//Called after RMB click
	void Shoot();

	//Called after pressing 1,2,3, mouse scrolling...
	void ChangeCurrentWeapon(uint8 Slot);

	//after pressing R, triggers a succession of RPCs to replicate that action
	void BeginReload();

	//Called by the AnimBP on montage to actually reload all the bullets
	UFUNCTION(BlueprintCallable)
	virtual void Reload();

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnAnimChange OnAnimChangeEvent;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnAnimTransientChange OnAnimTransientChangeEvent;

protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="HealthSystem")
	UHealthSystem *HealthSystem;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Weapon Inventory")
	UWeaponInventorySystem *WeaponInventory;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Key itens Inventory")
	UInventorySystem *Inventory;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="AI")
	UAIPerceptionStimuliSourceComponent *StimulusSource;
	
	virtual void BeginPlay() override;

	void BeginReloadVisuals();

	UFUNCTION(Server,Reliable)
	void BeginReloadServerSide();

	UFUNCTION(NetMulticast,Reliable)
    void BeginReloadClientSide();

	void SetCurrentPose(EAnimEnumPose Pose);

	

	//for networking
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	bool IsReloading;
	bool IsAiming;
	float CurrentSpeedMultiplier=0.5;
	
private:


	UPROPERTY(Replicated)
	bool CanCharacterMove=true;

	UFUNCTION(Server,Unreliable)
	void SetCurrentPoseServer(EAnimEnumPose Pose);
	
	//treated as input
	UPROPERTY(ReplicatedUsing=OnCurrentPoseChanged) 
	EAnimEnumPose CurrentPose;

	UFUNCTION()
	void OnCurrentPoseChanged();



	
	
	
};
