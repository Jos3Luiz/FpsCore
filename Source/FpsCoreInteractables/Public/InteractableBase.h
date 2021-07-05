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
	// Sets default values for this actor's properties
	AInteractableBase();

	virtual void OnInteract() PURE_VIRTUAL(AInteractableBase::OnInteract,);




};
