// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsPlayer.h"


#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractableBase.h" 
#include "Engine/World.h"

AFpsPlayer::AFpsPlayer()
{
	FpsCamera=CreateDefaultSubobject<UCameraComponent>(TEXT("FpsCamera"));
	FpsCamera->SetupAttachment(RootComponent);
	FpsCamera->bUsePawnControlRotation = true;

	FpsSkMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FpsSkMesh"));
	FpsSkMesh->SetupAttachment(FpsCamera);

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	FpsSkMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,ECollisionResponse::ECR_Block);
	
}

void AFpsPlayer::BeginPlay()
{
	Super::BeginPlay();
	WeaponInventory->OnWeaponAdded.AddDynamic(this,&AFpsPlayer::BindWeaponToHand);
	WeaponInventory->OnWeaponRemoved.AddDynamic(this,&AFpsPlayer::UnBindWeaponToHand);
	
}

void AFpsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Reload",IE_Pressed,this,&APlayerBase::BeginReload);
	//PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&APlayerBase::Shoot);
	PlayerInputComponent->BindAxis("FireHold",this,&AFpsPlayer::FireHold);
	PlayerInputComponent->BindAction("Aim",IE_Pressed,this,&AFpsPlayer::ZoomInAds);
	PlayerInputComponent->BindAction("Aim",IE_Released,this,&AFpsPlayer::ZoomOutAds);
	PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&AFpsPlayer::Interact);
	PlayerInputComponent->BindAction("DropWeapon",IE_Pressed,this,&AFpsPlayer::DropWeapon);
	
	//movement basic
	PlayerInputComponent->BindAxis("MoveForward",this,&AFpsPlayer::HandleMovementAxisForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AFpsPlayer::HandleMovementAxisRight);
	PlayerInputComponent->BindAxis("LookUp",this,&AFpsPlayer::HandleMovementAxisLookUp);
	PlayerInputComponent->BindAxis("Turn",this,&AFpsPlayer::HandleMovementAxisTurn);

	//movement advanced
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AFpsPlayer::SprintPressed);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AFpsPlayer::SprintReleased);
	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&AFpsPlayer::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch",IE_Released,this,&AFpsPlayer::EndCrouch);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AFpsPlayer::BeginJump);
	
	
	//weapon bindings
	PlayerInputComponent->BindAction("Weapon1",IE_Pressed,this,&AFpsPlayer::HandleChangeToWeaponOne);
	PlayerInputComponent->BindAction("Weapon2",IE_Pressed,this,&AFpsPlayer::HandleChangeToWeaponTwo);
	PlayerInputComponent->BindAction("Weapon3",IE_Pressed,this,&AFpsPlayer::HandleChangeToWeaponThree);
	PlayerInputComponent->BindAction("Weapon4",IE_Pressed,this,&AFpsPlayer::HandleChangeToWeaponFour);
}

void AFpsPlayer::UpdateCurrentLookingInteractable()
{
	AInteractableBase *Result;
	FCollisionShape StructureCollision = FCollisionShape::MakeSphere(40.0f);
	FVector StartTrace = FpsCamera->GetComponentLocation();
	FVector EndTrace = (FpsCamera->GetForwardVector() * 150.0f) + StartTrace;
	FHitResult HitResult;

	if(CurrentLookingInteractable)
	{
		//Disable Outline if not seen
		CurrentLookingInteractable->ChangeOutline(false);
		CurrentLookingInteractable = nullptr;
	}
	
	//ECC_GameTraceChannel1 = interactable channel
	bool HitSuccess = GetWorld()->SweepSingleByChannel(HitResult,EndTrace,EndTrace,FQuat::Identity,ECollisionChannel::ECC_GameTraceChannel1,StructureCollision);
	//DrawDebugSphere(GetWorld(),EndTrace,40.0f,50,FColor::Orange,false);
	if(HitSuccess)
	{
		//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), HitResult.Location, 20.f, 12, FColor::Red, false, 10.f);
		Result = Cast<AInteractableBase>(HitResult.Actor);
		if (Result)
		{
			CurrentLookingInteractable = Result;
			CurrentLookingInteractable->ChangeOutline(true);
		}
	}
}


void AFpsPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCurrentLookingInteractable();
}

void AFpsPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PossessedByClientSide();
	
}

void AFpsPlayer::PossessedByClientSide_Implementation()
{
	//disables the default mesh if in fps mode
	FpsSkMesh->SetSkeletalMesh(FpsSkReplace);
	FpsSkMesh->SetAnimInstanceClass(FpsAnimBpReplace->GeneratedClass);
	FpsSkMesh->SetCastShadow(false);
	
}


void AFpsPlayer::SprintPressed()
{
	CurrentSpeedMultiplier=0.8f;
}

void AFpsPlayer::SprintReleased()
{
	CurrentSpeedMultiplier=0.5f;
}

void AFpsPlayer::HandleMovementAxisRight(float Amount)
{
	if(GetCanCharacterMove())
	{
		AddMovementInput(GetActorRightVector()*Amount);
	}
}

void AFpsPlayer::HandleMovementAxisForward(float Amount)
{
	if(GetCanCharacterMove())
	{
		AddMovementInput(GetActorForwardVector()*Amount);
	}
}


void AFpsPlayer::MayInterruptAnim()
{
	if(IsReloading)
	{
		SetCurrentPose(EAnimEnumPose::AEP_Idle);
		IsReloading=false;
	}

}

void AFpsPlayer::HandleChangeToWeaponOne()
{
	ChangeCurrentWeapon(0);
}

void AFpsPlayer::HandleChangeToWeaponTwo()
{
	ChangeCurrentWeapon(1);
}

void AFpsPlayer::HandleChangeToWeaponThree()
{
	ChangeCurrentWeapon(2);
}

void AFpsPlayer::HandleChangeToWeaponFour()
{
	ChangeCurrentWeapon(3);
}

void AFpsPlayer::ZoomInAds()
{
	SetCurrentPose(EAnimEnumPose::AEP_AimDownSights);
}

void AFpsPlayer::ZoomOutAds()
{
	SetCurrentPose(EAnimEnumPose::AEP_Idle);
}

void AFpsPlayer::Interact()
{
	if(CurrentLookingInteractable)
	{
		CurrentLookingInteractable->Interact(this);
	}
	//if is a weapon, collect it
	AWeaponBase *WeaponToCollect = Cast<AWeaponBase>(CurrentLookingInteractable);
	if(WeaponToCollect)
	{
		WeaponInventory->ReplaceWeaponToInventory(WeaponToCollect);
	}
}

void AFpsPlayer::DropWeapon()
{
	WeaponInventory->RemoveWeaponFromInventory(WeaponInventory->GetCurrentSlot());
}

void AFpsPlayer::BindWeaponToHand(AWeaponBase *WeaponToCollect)
{
	if(WeaponToCollect)
	{
		WeaponToCollect->OnAddToInventory();	
		WeaponToCollect->AttachToComponent(FpsSkMesh,FAttachmentTransformRules::SnapToTargetIncludingScale,WeaponToCollect->GetSocketName());
			
	}

}

void AFpsPlayer::UnBindWeaponToHand(AWeaponBase *WeaponToRemove)
{
	if(WeaponToRemove)
	{
		WeaponToRemove->OnRemoveFromInventory();
		WeaponToRemove->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	}
}





void AFpsPlayer::HandleMovementAxisLookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void AFpsPlayer::HandleMovementAxisTurn(float Amount)
{
	AddControllerYawInput(Amount);
}

void AFpsPlayer::BeginCrouch()
{
	Crouch();
}

void AFpsPlayer::EndCrouch()
{
	UnCrouch();
}

void AFpsPlayer::BeginJump()
{
	Jump();
}

void AFpsPlayer::FireHold(float Amount)
{
	if (Amount > 0)
	{
		Shoot();
	}
}






