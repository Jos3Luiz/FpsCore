// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyItemStruct.h"
#include "InventorySystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSCORECOMPONENTS_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Replicated)
	TArray<FKeyItemStruct> KeyItens;

public:
	UFUNCTION(BlueprintCallable)
	void AddItem(FString ItemSecret,bool DestroyedAfterUse);
	
	UFUNCTION(BlueprintCallable)
	void AddItemStruct(FKeyItemStruct ItemStruct);

	//returns true if successfully
	UFUNCTION(BlueprintCallable)
	bool UseItem(FString ItemSecret);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
};
