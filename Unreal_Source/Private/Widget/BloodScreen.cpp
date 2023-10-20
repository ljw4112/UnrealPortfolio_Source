// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BloodScreen.h"

#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Utils/UIUtils.h"

void UBloodScreen::TurnOnBloodImage()
{
	UUIUtils::SetActive(BloodImage, true);
	PlayAnimation(GetHitAnim);
}
