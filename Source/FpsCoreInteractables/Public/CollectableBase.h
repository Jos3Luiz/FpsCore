// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "CollectableBase.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOREINTERACTABLES_API ACollectableBase : public AInteractableBase
{
	GENERATED_BODY()

public:
	virtual void OnRemoveFromInventory(){};
	virtual void OnAddToInventory(){};
	
};
