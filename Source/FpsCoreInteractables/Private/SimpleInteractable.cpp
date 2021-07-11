// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleInteractable.h"

ASimpleInteractable::ASimpleInteractable()
{
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interactable Mesh"));
	SetRootComponent(InteractableMesh);
	//ECC_GameTraceChannel1 = interactable
	InteractableMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,ECollisionResponse::ECR_Block);
}

/*
void ASimpleInteractable::Interact(AActor *AInstigator)
{
	OnInteractEvent.Broadcast();
}
*/

void ASimpleInteractable::ChangeOutline(bool Val)
{
	InteractableMesh->SetRenderCustomDepth(Val);
}




