// Fill out your copyright notice in the Description page of Project Settings.
#include "Firearm.h"

#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "BulletDamageType.h"
#include "AttachmentEnum.h"


AFirearm::AFirearm()
{
	AttachmentRefList.SetNum(StaticCast<int>(EAttachmentTypeEnum::AT_Max));
	SocketName="Firearm";
	IdleMontageSectionName="Firearm";
	ReloadMontageSectionName="Firearm";
	
}

void AFirearm::BeginPlay()
{
	Super::BeginPlay();
	AddAttachment_Implementation(DefaultSight);
	
}

void AFirearm::AddAttachment_Implementation(TSubclassOf<AAttachmentBase> NewAttachment)
{
	//check if is allowed and if isnt none
	if (!(AttachmentAllowedList.Contains(NewAttachment) && NewAttachment) )
	{
		return;
	}
	//spawns new item
	AAttachmentBase *Spawned =  Cast<AAttachmentBase>(GetWorld()->SpawnActor(NewAttachment));
	int Index = static_cast<int>( Spawned->AttachmentType);

	//if there was an old attachment
	AAttachmentBase *OldAttachment = AttachmentRefList[Index];
	if(OldAttachment)
	{
		GetWorld()->DestroyActor(OldAttachment);
	}
	
	AttachmentRefList[Index] = Spawned;
	//attaches new attachment into the correct socket
	Spawned->AttachToActor(this,FAttachmentTransformRules::SnapToTargetIncludingScale,Spawned->SocketName);
	
}

void AFirearm::Shoot(AActor* AInstigator)
{
	if (!IsOnCooldown() && CurrentAmmo>0)
	{
		Super::Shoot(AInstigator);
		const FVector Start = Weapon->GetSocketLocation(TEXT("Muzzle"));
		const FVector Direction = GetActorForwardVector();
		
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
		CurrentAmmo--;

	}
}


void AFirearm::VisualShootLogic(AActor* AInstigator, FVector Start, FVector Direction)
{
	Super::VisualShootLogic(AInstigator, Start, Direction);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),BulletSystem, Weapon->GetSocketLocation(TEXT("Muzzle")), Weapon->GetComponentRotation());
}

void AFirearm::ReloadWeapon()
{
	int Needed= FMath::Clamp(MaxAmmo - CurrentAmmo,0,CurrentStockAmmo);
	CurrentAmmo+=Needed;
	CurrentStockAmmo-=Needed;
}

bool AFirearm::CanReloadWeapon()
{
	return (CurrentAmmo < MaxAmmo) && (CurrentStockAmmo > 0);
}

void AFirearm::Destroyed()
{
	for (AAttachmentBase *AttachmentRef : AttachmentRefList)
	{
		if(AttachmentRef)
		{
			AttachmentRef->Destroy();
		}
		
	}
}

void AFirearm::SetWeaponVisibility(bool Val)
{
	Super::SetWeaponVisibility(Val);
	for (AAttachmentBase *AttachmentRef : AttachmentRefList)
	{
		if(AttachmentRef)
		{
			AttachmentRef->SetActorHiddenInGame(!Val);	
		}
	}
}

void AFirearm::GetAttachmentSightDetails(FTransform& AimPosition, float& ZoomMultiplier)
{
	ASightAttachment *Sight;
	int Index = static_cast<int>(EAttachmentTypeEnum::AT_Sight);
	Sight = Cast<ASightAttachment>(AttachmentRefList[Index]);
	if(Sight)
	{
		ZoomMultiplier = Sight->ZoomMultiplier;
		AimPosition = Sight->AttachmentMesh->GetSocketTransform("sight");
	}
}


