// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

UCLASS(Abstract)
class FPSCOREINTERACTABLES_API AInteractableBase : public AActor
{
	GENERATED_BODY()
public:	
	virtual void Interact(AActor *AInstigator){
		OnInteractBP(AInstigator);
	};
	
	virtual void ChangeOutline(bool Val){};

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractBP(AActor *AInstigator);


};
