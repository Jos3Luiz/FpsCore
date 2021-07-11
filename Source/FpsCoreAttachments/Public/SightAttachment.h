// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttachmentBase.h"
#include "AttachmentEnum.h"
#include "SightAttachment.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSCOREATTACHMENTS_API ASightAttachment : public AAttachmentBase
{
	GENERATED_BODY()
	
	ASightAttachment()
	{
		AttachmentType = EAttachmentTypeEnum::AT_Sight;
	}
public:


	//defines the amount of zoom applied to the camera when aiming down sights
	UPROPERTY(VisibleAnywhere)
	float ZoomMultiplier;
};
