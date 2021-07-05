// Fill out your copyright notice in the Description page of Project Settings.
#include "Firearm.h"

#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "BulletDamageType.h"


AFirearm::AFirearm()
{
	
}

void AFirearm::BeginPlay()
{
	Super::BeginPlay();
	SetupAttachments();
}

void AFirearm::Shoot(AActor* AInstigator)
{
	if (!IsOnCooldown() && CurrentAmmo>0)
	{
		Super::Shoot(AInstigator);
		const FVector Start = Weapon->GetSocketLocation(TEXT("Muzzle"));
		const FVector Direction = GetActorForwardVector();

		ApplyRecoil();
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

void AFirearm::ServerShootLogic(AActor* AInstigator, FVector Start, FVector Direction)
{
	
	FHitResult HitDetails = FHitResult(ForceInit);
	FDamageEvent DamageEvent;
	
	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;
	
	bool HitSuccess = GetWorld()->LineTraceSingleByChannel(HitDetails,Start,Start+Direction*1000000,ECollisionChannel::ECC_Visibility,TraceParams);
	
	//DrawDebugLine(GetWorld(), start, start+direction*10000, FColor::Red, false, 5.f, ECC_WorldStatic, 1.f);
	if (HitSuccess)
	{

		UGameplayStatics::ApplyPointDamage(HitDetails.Actor.Get(),WeaponDamage,Start,HitDetails,GetOwner()->GetInstigatorController(),this,UBulletDamageType::StaticClass());
		
	}

}


void AFirearm::VisualShootLogic(AActor* AInstigator, FVector Start, FVector Direction)
{

	FHitResult HitDetails = FHitResult(ForceInit);
	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),BulletSystem, Weapon->GetSocketLocation(TEXT("Muzzle")), Weapon->GetComponentRotation());
	
	bool HitSucess = GetWorld()->LineTraceSingleByChannel(HitDetails,Start,Start+Direction*10000,ECollisionChannel::ECC_Visibility,TraceParams);
	if (HitSucess)
	{
		//ACharacter *DamagedActor = Cast<ACharacter>(HitDetails.Actor);
		//spawn damage particles here
	}
	
}
void AFirearm::ReloadWeapon()
{
	int Needed= FMath::Clamp(MaxAmmo - CurrentAmmo,0,MaxAmmo);
	CurrentAmmo+=Needed;
	CurrentStockAmmo-=Needed;
	OnAmmoChangedEvent.Broadcast(CurrentAmmo,CurrentStockAmmo);
}



void AFirearm::SetRecoilY()
{
	float randomRecoil = FMath::RandRange(-0.1f,-0.05f);
	float burstMultiplier = FMath::Clamp(RecoilTotalY*-1,1.0f,1.5f);
	float currentRecoilY = randomRecoil * burstMultiplier * RecoilMultiplierVertical;
	if (RecoilTotalY < -12.0f)
	{
		currentRecoilY*=0.1f;
	}
	RecoilTotalY+=currentRecoilY;
	UGameplayStatics::GetPlayerController(GetWorld(),0)->AddPitchInput(currentRecoilY);
}

void AFirearm::SetRecoilX()
{
	float randomRecoil = FMath::RandRange(-0.05f,0.1f);
	float burstMultiplier = FMath::Clamp(RecoilTotalY,1.0f,2.0f);
	float currentRecoilX = randomRecoil * burstMultiplier * RecoilMultiplierHorizontal;
	if ((RecoilTotalX < -12.0f && currentRecoilX < 0.0f) || (RecoilTotalX > 12.0f && currentRecoilX > 0.0f))
	{
		currentRecoilX *=-2.0f;
	}
	if(RecoilTotalY<-12.0f)
	{
		currentRecoilX *=3;
	}
	RecoilTotalX+=currentRecoilX;
	UGameplayStatics::GetPlayerController(GetWorld(),0)->AddYawInput(currentRecoilX);
}

void AFirearm::BackRecoil(float timelineState)
{
	float newInputY= RecoilTotalY*timelineState*-1.0f;
	UGameplayStatics::GetPlayerController(GetWorld(),0)->AddPitchInput(newInputY);
	RecoilTotalY=RecoilTotalY + newInputY;
	
	float newInputX= RecoilTotalX*timelineState*-1.0f;
	UGameplayStatics::GetPlayerController(GetWorld(),0)->AddYawInput(newInputX);
	RecoilTotalX=RecoilTotalX+newInputX;

}




void AFirearm::SetupAttachments()
{
	
	//AAttachmentBase *CurrAttachment;
	
	for (TSubclassOf<AAttachmentBase> DesiredAttachment : AttachmentList)
	{
		//fix me
		//CurrAttachment = GetWorld()->SpawnActor<DesiredAttachment>(DesiredAttachment.Get(), FVector(), FRotator(),FActorSpawnParameters() );
		//SightAttachment->AttachToActor(this,FAttachmentTransformRules::SnapToTargetIncludingScale,TEXT("sight"));	
		
	}

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

void AFirearm::SetWeaponVisibilityCallback(bool Val)
{
	Super::SetWeaponVisibilityCallback(Val);
	for (AAttachmentBase *AttachmentRef : AttachmentRefList)
	{
		if(AttachmentRef)
		{
			AttachmentRef->SetActorHiddenInGame(!Val);	
		}
	}
}


