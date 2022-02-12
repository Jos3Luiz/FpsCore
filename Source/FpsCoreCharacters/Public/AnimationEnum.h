#pragma once

#include "CoreMinimal.h"

UENUM( BlueprintType )
enum class EAnimEnumPose : uint8
{
	AEP_Idle			UMETA( DisplayName = "Idle" ),
    AEP_AimDownSights	UMETA( DisplayName = "AimDownSights" ),
	AEP_Crouch	UMETA( DisplayName = "Crouch" ),
	AEP_Prone	UMETA( DisplayName = "Prone" ),

    AEP_Max		UMETA( Hidden )
};

UENUM( BlueprintType )
enum class EAnimEnumTransient : uint8
{
    AET_Shoot			UMETA( DisplayName = "Shoot" ),
	AET_DryShoot			UMETA( DisplayName = "DryShoot" ),
	AET_Reload			UMETA( DisplayName = "Reload" ),
    AET_Max		UMETA( Hidden )
};