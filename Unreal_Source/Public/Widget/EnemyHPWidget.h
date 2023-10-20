// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UEnemyHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;

	void SetPercent(float Percent);
};
