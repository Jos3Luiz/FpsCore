// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachmentBase.h"

// Sets default values
AAttachmentBase::AAttachmentBase()
{
	PrimaryActorTick.bCanEverTick = false;
	AttachmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachmentMesh"));
	SetRootComponent(AttachmentMesh);
	AttachmentMesh->SetCastShadow(false);

}


