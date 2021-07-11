#pragma once

#include "CoreMinimal.h"

UENUM( BlueprintType )
enum class EAttachmentTypeEnum : uint8
{
	AT_Sight			UMETA( DisplayName = "Sight" ),
	AT_Barrel			UMETA( DisplayName = "Barrel" ),

    AT_Max		UMETA( Hidden )
};
