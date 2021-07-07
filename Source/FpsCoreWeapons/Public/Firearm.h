// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "SightAttachment.h"
#include "AttachmentBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PointLightComponent.h"
#include "Firearm.generated.h"

/**
 * A firearm should have the following sockets in its skeleton mesh to work properly
 * Muzzle
 */
UCLASS(Abstract,Blueprintable,BlueprintType)
class FPSCOREWEAPONS_API AFirearm : public AWeaponBase
{
	GENERATED_BODY()

	AFirearm();

public:
	virtual void BeginPlay() override;

	virtual void ReloadWeapon() override;

protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float AimTime=0.1f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float HitImpulseAmmount=400.0f;
	
	virtual void Destroyed() override;
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
    void ApplyRecoil();

	UFUNCTION(BlueprintCallable)
    void SetRecoilY();
	
	UFUNCTION(BlueprintCallable)
    void SetRecoilX();

	UFUNCTION(BlueprintCallable)
    void BackRecoil(float TimelineState);

	UPROPERTY(BlueprintReadWrite)
	float RecoilTotalY;

	UPROPERTY(BlueprintReadWrite)
	float RecoilTotalX;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent *PlanarReflex;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UPointLightComponent *PointLight;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RecoilMultiplierVertical=1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RecoilMultiplierHorizontal=1;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem *BulletSystem;
	

	


	UPROPERTY()
	TArray<AAttachmentBase *>AttachmentRefList;

	TArray<TSubclassOf<AAttachmentBase>>AttachmentList;

	//------------------------ overrides


	virtual void VisualShootLogic(AActor *AInstigator,FVector Start,FVector Direction) override;

	virtual void ServerShootLogic(AActor *AInstigator,FVector Start,FVector Direction) override;

	virtual void SetWeaponVisibility(bool Val) override;

	virtual void Shoot(AActor* AInstigator) override;

	virtual bool CanReloadWeapon() override;

	
private:
	void SetupAttachments();
	
};
