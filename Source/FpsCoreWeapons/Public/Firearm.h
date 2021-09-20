// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "SightAttachment.h"
#include "AttachmentBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PointLightComponent.h"
#include "Curves/CurveVector.h"
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

	UFUNCTION(BlueprintPure)
	virtual void GetAttachmentSightDetails(FTransform &AimPosition,float &ZoomMultiplier);

protected:
	UPROPERTY(EditDefaultsOnly,Category="WeaponProperties")
	TSubclassOf<ASightAttachment> DefaultSight;

	UPROPERTY(EditDefaultsOnly,Category="WeaponProperties")
	UStaticMeshComponent* DefaultMagazine;
	
	UPROPERTY(EditAnywhere,Category="WeaponProperties")
	TArray<TSubclassOf<AAttachmentBase>>AttachmentAllowedList;
	
	UPROPERTY(EditAnywhere,Category="WeaponProperties")
	UNiagaraSystem *BulletSystem;
	
	UPROPERTY(EditAnywhere,Category="WeaponProperties")
	UCurveVector *RecoilCurve;
	
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UStaticMeshComponent *PlanarReflex;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UPointLightComponent *PointLight;

	UPROPERTY()
	TArray<AAttachmentBase *>AttachmentRefList;

	UFUNCTION(Server,Reliable,BlueprintCallable)
	void AddAttachment(TSubclassOf<AAttachmentBase> NewAttachment);

	//------------------------ overrides

	virtual void Destroyed() override;
	
	virtual void VisualShootLogic(AActor *AInstigator,FVector Start,FVector Direction) override;

	virtual void SetWeaponVisibility(bool Val) override;

	virtual void Shoot(AActor* AInstigator) override;

	virtual bool CanReloadWeapon() override;

	

};
