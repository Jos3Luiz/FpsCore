// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "FpsPlayer.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSCORECHARACTERS_API AFpsPlayer : public APlayerBase
{
	GENERATED_BODY()
	
public:
	AFpsPlayer();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	USkeletalMesh *FpsSkReplace;

	UPROPERTY(EditAnywhere)
	UAnimBlueprint *FpsAnimBpReplace;
	
protected:
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;

	void UpdateCurrentLookingInteractable();

	UPROPERTY()
	AInteractableBase *CurrentLookingInteractable=nullptr;
	

	UFUNCTION(Client,Reliable)
	void PossessedByClientSide();
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UCameraComponent *FpsCamera;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USkeletalMeshComponent *FpsSkMesh;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE void SprintPressed();
	FORCEINLINE void SprintReleased();

	FORCEINLINE void HandleChangeToWeaponOne();
	FORCEINLINE void HandleChangeToWeaponTwo();
	FORCEINLINE void HandleChangeToWeaponThree();
	FORCEINLINE void HandleChangeToWeaponFour();
	
	FORCEINLINE void HandleMovementAxisRight(float Amount);
	FORCEINLINE void HandleMovementAxisForward(float Amount);
	FORCEINLINE void HandleMovementAxisLookUp(float Amount);
   	FORCEINLINE void HandleMovementAxisTurn(float Amount);
	FORCEINLINE void FireHold(float Amount);

	FORCEINLINE void BeginCrouch();
	FORCEINLINE void EndCrouch();
	FORCEINLINE void BeginJump();
	
	void MayInterruptAnim();

	void ZoomInAds();
	void ZoomOutAds();
	void Interact();
	void DropWeapon();

	UFUNCTION() //needed because of the AddDynamic
	void BindWeaponToHand(AWeaponBase* WeaponToCollect);

	UFUNCTION()//needed because of the AddDynamic
	void UnBindWeaponToHand(AWeaponBase* WeaponToRemove);
	
	
	
	
	
};
