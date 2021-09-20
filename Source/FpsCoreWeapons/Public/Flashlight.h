// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Components/SpotLightComponent.h"
#include "Net/UnrealNetwork.h"

#include "Flashlight.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOREWEAPONS_API AFlashlight : public AWeaponBase
{
	GENERATED_BODY()
	AFlashlight();
protected:

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	UStaticMeshComponent *FlashlightMesh;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	USpotLightComponent *FlashlightSpotLight;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float FlashlightBrightness;
	
	UPROPERTY(ReplicatedUsing=OnRepFlashOn)
	bool FlashOn;

	UFUNCTION()
	void OnRepFlashOn(bool IsOn);

	UFUNCTION(Server,Unreliable)
	void UpdateFlashlightServer(bool IsEnabled);
	
	virtual void Shoot(AActor* AInstigator) override;
	virtual void ServerShootLogic(AActor* AInstigator, FVector Start, FVector Direction) override{};
	virtual void VisualShootLogic(AActor* AInstigator, FVector Start, FVector Direction) override{};
	virtual void ChangeOutline(bool Val) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override{return true;};

	virtual void OnAddToInventory() override;
	virtual void OnRemoveFromInventory() override;
};
