// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"
#include "Net/UnrealNetwork.h"

void UInventorySystem::AddItem(FString ItemSecret, bool DestroyedAfterUse)
{
	FKeyItemStruct NewStruct;
	NewStruct.KeySecret=ItemSecret;
	NewStruct.DestroyedAfterUse=DestroyedAfterUse;
	KeyItens.AddUnique(NewStruct);
	
}

void UInventorySystem::AddItemStruct(FKeyItemStruct ItemStruct)
{
	KeyItens.AddUnique(ItemStruct);
}

bool UInventorySystem::UseItem(FString ItemSecret)
{
	for(FKeyItemStruct Item :KeyItens)
	{
		if (Item.KeySecret==ItemSecret)
		{
			return true;
		}
	}
	return false;
}

void UInventorySystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventorySystem, KeyItens);
}

bool UInventorySystem::IsSupportedForNetworking() const
{
	return true;
}
