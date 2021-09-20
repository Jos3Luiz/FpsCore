// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum EAmmoTypes
{
  AT_Pistol_9     UMETA(DisplayName = "Pistol 9 milimiters"),
  AT_Pistol_45     UMETA(DisplayName = "Pistol 45 milimiters"),
  AT_Rifle_556     UMETA(DisplayName = "Rifle 556 milimiters"),
  AT_Rifle_752     UMETA(DisplayName = "Rifle 762 milimiters"),
  AT_Shotgun_12     UMETA(DisplayName = "Shotgun 12 Gage"),
  AT_Size          UMETA(DisplayName = "Unused"),
};

UENUM()
enum EWeaponSlot
{
  WS_Primary      UMETA(DisplayName = "Primary"),
  WS_Secondary     UMETA(DisplayName = "Secondary"),
  WS_Melee         UMETA(DisplayName = "Melee"),
  WS_Alternative   UMETA(DisplayName = "Alternative"),
  WS_Size          UMETA(DisplayName = "Unused"),
};
