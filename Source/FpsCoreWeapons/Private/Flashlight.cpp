// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"


AFlashlight::AFlashlight()
{
	FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flashlight"));
	FlashlightMesh->SetupAttachment(RootComponent);
	FireTime=0;
	WeaponDamage=0;
	MaxEffectiveRange=0;
	SocketName="Flashlight";
	IdleMontageSectionName="Flashlight";
	ReloadMontageSectionName="Flashlight";
}

void AFlashlight::Shoot(AActor* AInstigator)
{
	//no need to cooldown, use on demand
	//Super::Shoot(AInstigator);
	FlashOn=!FlashOn;
	UpdateFlashlightServer(FlashOn);
	OnRepFlashOn(FlashOn);
}

void AFlashlight::ChangeOutline(bool Val)
{
	FlashlightMesh->SetRenderCustomDepth(Val);
}

void AFlashlight::OnRepFlashOn(bool IsOn)
{
	FlashlightSpotLight->SetLightBrightness(FlashlightBrightness * StaticCast<int>(IsOn));
}

void AFlashlight::UpdateFlashlightServer_Implementation(bool IsEnabled)
{
	FlashOn=IsEnabled;
}
void AFlashlight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AFlashlight,FlashOn,COND_SkipOwner);
}
void AFlashlight::OnAddToInventory()
{
	Weapon->SetSimulatePhysics(false);
	SetActorEnableCollision(false);
}

void AFlashlight::OnRemoveFromInventory()
{
	Weapon->SetSimulatePhysics(true);
	SetActorEnableCollision(true);
}






