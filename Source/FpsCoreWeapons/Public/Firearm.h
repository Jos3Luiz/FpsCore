// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "SightAttachment.h"
#include "AttachmentBase.h"
#include "Curves/CurveVector.h"
#include "Firearm.generated.h"


UENUM( BlueprintType )
enum class EShootType : uint8
{
	AST_Shoot			UMETA( DisplayName = "Shoot" ),
	AST_DryShoot			UMETA( DisplayName = "DryShoot" ),
	AST_Max		UMETA( Hidden )
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnShoot, EShootType,ActionType );
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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponProperties")
	UStaticMesh *DefaultMagazine;
	
	UPROPERTY(EditAnywhere,Category="WeaponProperties")
	TArray<TSubclassOf<AAttachmentBase>>AttachmentAllowedList;
	
	UPROPERTY(EditAnywhere,Category="WeaponProperties")
	UCurveVector *RecoilCurve;

	UPROPERTY(BlueprintReadOnly,Category="WeaponProperties")
	UStaticMeshComponent *Magazine;
	
	UPROPERTY()
	TArray<AAttachmentBase *>AttachmentRefList;

	UPROPERTY(BlueprintAssignable)
	FDelegate_OnShoot OnShoot;

	UFUNCTION(Server,Reliable,BlueprintCallable)
	void AddAttachment(TSubclassOf<AAttachmentBase> NewAttachment);

	//------------------------ overrides

	virtual void Destroyed() override;
	
	virtual void VisualShootLogic(AActor *AInstigator,FVector Start,FVector Direction) override;

	virtual void SetWeaponVisibility(bool Val) override;

	virtual void Shoot(AActor* AInstigator) override;

	virtual void DryShoot() override;

	virtual bool CanReloadWeapon() override;

	

};
