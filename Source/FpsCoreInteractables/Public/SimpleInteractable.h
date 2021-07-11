// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "SimpleInteractable.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

UCLASS(Abstract)
class FPSCOREINTERACTABLES_API ASimpleInteractable : public AInteractableBase
{
	GENERATED_BODY()
	ASimpleInteractable();
	
public:
	//virtual void Interact(AActor *AInstigator) override;
	virtual void ChangeOutline(bool Val) override;


	//UPROPERTY(BlueprintAssignable)
	//FOnInteract OnInteractEvent;
	

protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UStaticMeshComponent *InteractableMesh;
	
};
