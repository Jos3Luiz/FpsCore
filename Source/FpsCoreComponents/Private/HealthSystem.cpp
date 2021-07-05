// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthSystem.h"
#include "Net/UnrealNetwork.h"
#include "BulletDamageType.h"

// Sets default values for this component's properties
UHealthSystem::UHealthSystem()
{
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UHealthSystem::BeginPlay()
{
	Super::BeginPlay();
	Health=DefaultHealth;
	
	AActor *Owner= GetOwner();
	if(Owner)
	{
		Owner->OnTakePointDamage.AddDynamic(this,&UHealthSystem::TakeDamage);
	}
	
}


void UHealthSystem::TakeDamage_Implementation(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
													class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
													const class UDamageType* DamageType, AActor* DamageCauser)
{
	if(GetOwner()->HasAuthority())
	{
		if(Cast<UBulletDamageType>(DamageType) && BoneName=="head")
		{
			Damage=Damage*HeadShootMultiplier;
		}
		Health= FMath::Clamp(Health - Damage,0.0f,DefaultHealth);

		OnHealthChangedEvent.Broadcast(Health);
		if (Health==0)
		{
			OnDieEvent.Broadcast();	
		}
	}
	
}

void UHealthSystem::OnRep_HealthUpdated()
{
	OnHealthChangedEvent.Broadcast(Health);
	if (Health==0)
	{
		OnDieEvent.Broadcast();	
	}
}


void UHealthSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthSystem, Health);
}

bool UHealthSystem::IsSupportedForNetworking() const
{
	return true;
}