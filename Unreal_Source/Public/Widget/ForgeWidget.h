// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ForgeWidget.generated.h"

DECLARE_DELEGATE(FWidgetClose)

UCLASS()
class PORTFOLIO01_API UForgeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FWidgetClose OnCloseFunc;
	
	UFUNCTION(BlueprintImplementableEvent)
	void RefreshListElement();

	UFUNCTION(BlueprintCallable)
	int32 GetItemCount(FName ItemName);

	UFUNCTION(BlueprintCallable)
	bool OnClose();
};
