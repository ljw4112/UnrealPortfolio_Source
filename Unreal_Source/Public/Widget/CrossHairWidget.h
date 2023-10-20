// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrossHairWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UCrossHairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* CenterDot;
	
	UPROPERTY(meta = (BindWidget))
	UImage* CrossHair_North;

	UPROPERTY(meta = (BindWidget))
	UImage* CrossHair_South;

	UPROPERTY(meta = (BindWidget))
	UImage* CrossHair_East;

	UPROPERTY(meta = (BindWidget))
	UImage* CrossHair_West;

	void virtual NativeConstruct() override;
	
	void SetAttackPosition();
	void SetIdlePosition();

private:
	TArray<UImage*> CrossHairList;
	
	FTimerHandle OpacityHandle;
	void SetCrossHairOpacity(float Destination, bool bIncrease);
};
