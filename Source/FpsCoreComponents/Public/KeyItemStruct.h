
#pragma once


#include "CoreMinimal.h"
#include "KeyItemStruct.generated.h"

//defines the format of a quest item that should not be used directly in the hands of the player such as keys or important events that are unlocked
USTRUCT(BlueprintType) struct FKeyItemStruct
{
	GENERATED_BODY()

	UPROPERTY()
	bool DestroyedAfterUse=false;


	UPROPERTY()
	FString KeySecret;

	//allows for AddUnique equal operation 
	bool operator==(const FKeyItemStruct& OtherItem) const
	{
		return KeySecret==OtherItem.KeySecret;
    }

};

