// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee.h"

#include "BulletDamageType.h"
#include "Kismet/GameplayStatics.h"

AMelee::AMelee()
{
	MaxEffectiveRange=100;
	WeaponDamageType = UMeleeDamageType::StaticClass();
	SocketName="Melee";
	IdleMontageSectionName="Melee";
	ReloadMontageSectionName="Melee";
	FireTime=3.0f;
}

void AMelee::Shoot(AActor* AInstigator)
{
	if (!IsOnCooldown())
	{
		Super::Shoot(AInstigator);
		const FVector Start = GetActorLocation();
		const FVector Direction = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorForwardVector();

		VisualShootLogic(AInstigator,Start,Direction);
	
		if (HasAuthority())
		{
			ServerShootLogic(AInstigator,Start,Direction);
			ClientSideShoot(AInstigator,Start,Direction);
		}
		else
		{
			ServerSideShoot(AInstigator,Start,Direction);
		}	

	}
}