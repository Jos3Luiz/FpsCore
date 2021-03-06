// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttachmentEnum.h"
#include "AttachmentBase.generated.h"

UCLASS(Abstract)
class FPSCOREATTACHMENTS_API AAttachmentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttachmentBase();

	//mesh for the attachment
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
    UStaticMeshComponent *AttachmentMesh;
    
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName SocketName;
    
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	EAttachmentTypeEnum AttachmentType;
	

};
