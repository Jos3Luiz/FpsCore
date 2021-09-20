// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Melee.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOREWEAPONS_API AMelee : public AWeaponBase
{
	GENERATED_BODY()
	AMelee();
	
public:
	virtual bool CanReloadWeapon() override{ return false; }
	virtual void Shoot(AActor* AInstigator) override;
	
};
