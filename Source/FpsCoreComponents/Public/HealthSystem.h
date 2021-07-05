// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged,float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSCORECOMPONENTS_API UHealthSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthSystem();
	
	UPROPERTY(BlueprintAssignable,BlueprintCallable)
    FOnHealthChanged OnHealthChangedEvent;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnDie OnDieEvent;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	float DefaultHealth;

	UPROPERTY(ReplicatedUsing =OnRep_HealthUpdated, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere);
	float HeadShootMultiplier=2;
	
	UFUNCTION(Server,Reliable)
    void TakeDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
    void OnRep_HealthUpdated();

	//set health as replicated
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	//returns true for allowing network support
	virtual bool IsSupportedForNetworking() const override;
};
