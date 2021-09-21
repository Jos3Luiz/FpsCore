
#include "PlayerBase.h"

#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Components/CapsuleComponent.h"

APlayerBase::APlayerBase()
{
	bReplicates = true;
	HealthSystem = CreateDefaultSubobject<UHealthSystem>(TEXT("HealthSystem"));
	WeaponInventory=CreateDefaultSubobject<UWeaponInventorySystem>(TEXT("WeaponInventory"));
	Inventory=CreateDefaultSubobject<UInventorySystem>(TEXT("Inventory"));

	
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionSource"));
	StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimulusSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());

	//setting collision properly to mesh
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);


	//setting collision properly to capsule component
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic,ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Block);

}

void APlayerBase::SetCanCharacterMove(const bool Val)
{
	if(HasAuthority())
	{
		CanCharacterMove=Val;
	}
}

bool APlayerBase::GetCanCharacterMove()
{
	return CanCharacterMove;
}

UWeaponInventorySystem* APlayerBase::GetPlayerWeaponInventory()
{
	return WeaponInventory;
}




// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerBase::Reload()
{
	WeaponInventory->Reload();
	IsReloading=false;
}

void APlayerBase::BeginReload()
{
	BeginReloadVisuals();
	BeginReloadServerSide();
}

void APlayerBase::BeginReloadServerSide_Implementation()
{
	BeginReloadVisuals();
	BeginReloadClientSide();
}

void APlayerBase::BeginReloadClientSide_Implementation()
{
	if(!IsLocallyControlled())
	{
		BeginReloadVisuals();
	}
}

void APlayerBase::BeginReloadVisuals()
{

		OnAnimTransientChangeEvent.Broadcast(EAnimEnumTransient::AET_Reload);
		//add visual replication here
}

void APlayerBase::Shoot()
{
	if(WeaponInventory->Shoot())
	{
		//local effect only
		OnAnimTransientChangeEvent.Broadcast(EAnimEnumTransient::AET_Shoot);		
	}

}

void APlayerBase::ChangeCurrentWeapon(uint8 Slot)
{
	WeaponInventory->ChangeCurrentWeaponIndex(Slot);
}

void APlayerBase::SetCurrentPose(EAnimEnumPose Pose)
{
	CurrentPose=Pose;
	OnAnimChangeEvent.Broadcast(CurrentPose);
	
}
void APlayerBase::SetCurrentPoseServer_Implementation(EAnimEnumPose Pose)
{
	CurrentPose=Pose;
	OnAnimChangeEvent.Broadcast(CurrentPose);
}

void APlayerBase::OnCurrentPoseChanged()
{
	OnAnimChangeEvent.Broadcast(CurrentPose);
}





//for networking
void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerBase, CanCharacterMove);
	DOREPLIFETIME_CONDITION(APlayerBase,CurrentPose,COND_SkipOwner);
}

bool APlayerBase::IsSupportedForNetworking() const
{
	return true;
}









