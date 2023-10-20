// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHud.generated.h"

class UForgeWidget;
class UBloodScreen;
class UInventoryWidget;
class UInteractWidget;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UMainHud : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	UUserWidget* Crosshair;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* Interact;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* Inventory;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* BloodScreen;
	
	UPROPERTY(meta = (BindWidget))
	UUserWidget* Forge;

	UPROPERTY()
	UInteractWidget* InteractWidget;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY()
	UBloodScreen* BloodScreenWidget;

	UPROPERTY()
	UForgeWidget* ForgeWidget;

	void InitializeUI();
};
