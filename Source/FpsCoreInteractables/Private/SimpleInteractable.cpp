// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleInteractable.h"

ASimpleInteractable::ASimpleInteractable()
{
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interactable Mesh"));
	SetRootComponent(InteractableMesh);
	//ECC_GameTraceChannel1 = interactable
	InteractableMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,ECollisionResponse::ECR_Block);
}

void ASimpleInteractable::Interact()
{
	OnInteractEvent.Broadcast();
}

void ASimpleInteractable::EnableOutline()
{
	InteractableMesh->SetRenderCustomDepth(true);
}

void ASimpleInteractable::DisableOutline()
{
	InteractableMesh->SetRenderCustomDepth(false);
}





