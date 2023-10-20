#include "Widget/CrossHairWidget.h"

#include "Utils/UIUtils.h"
#include "Components/Image.h"

void UCrossHairWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CrossHairList.AddUnique(CrossHair_North);
	CrossHairList.AddUnique(CrossHair_South);
	CrossHairList.AddUnique(CrossHair_East);
	CrossHairList.AddUnique(CrossHair_West);

	for (auto Image : CrossHairList)
	{
		UUIUtils::SetActive(Image, false);
		Image->SetOpacity(0.f);
	}
}

void UCrossHairWidget::SetAttackPosition()
{
	for (int i = 0; i < CrossHairList.Num(); i++)
	{
		UUIUtils::SetActive(CrossHairList[i], true);
	}

	FTimerDelegate OpacityDelegate;
	OpacityDelegate.BindUObject(this, &UCrossHairWidget::SetCrossHairOpacity, 100.f, true);
	GetWorld()->GetTimerManager().SetTimer(OpacityHandle, OpacityDelegate, GetWorld()->DeltaTimeSeconds, true);
}

void UCrossHairWidget::SetIdlePosition()
{
	for (auto Image : CrossHairList)
	{
		UUIUtils::SetActive(Image, false);
		Image->SetOpacity(0.f);
	}

	for (int i = 0; i < CrossHairList.Num(); i++)
	{
		UUIUtils::SetActive(CrossHairList[i], false);
		CrossHairList[i]->SetOpacity(0.f);
	}

	FTimerDelegate OpacityDelegate;
	OpacityDelegate.BindUObject(this, &UCrossHairWidget::SetCrossHairOpacity, 0.f, false);
	GetWorld()->GetTimerManager().SetTimer(OpacityHandle, OpacityDelegate, GetWorld()->DeltaTimeSeconds, true);
}

void UCrossHairWidget::SetCrossHairOpacity(float Destination, bool bIncrease)
{
	if (bIncrease)
	{
		for (int i = 0; i <= CrossHairList.Num(); i++)
		{
			if (i == 0) continue;
			
			CrossHairList[i - 1]->SetOpacity(CrossHairList[i - 1]->GetColorAndOpacity().A + 5);
			if (CrossHairList[i - 1]->GetColorAndOpacity().A >= Destination) break;
		}

		GetWorld()->GetTimerManager().ClearTimer(OpacityHandle);
	}
	else
	{
		for (int i = 0; i <= CrossHairList.Num(); i++)
		{
			if (i == 0) continue;
			
			CrossHairList[i - 1]->SetOpacity(CrossHairList[i - 1]->GetColorAndOpacity().A - 5);
			if (CrossHairList[i - 1]->GetColorAndOpacity().A <= Destination) break;
		}

		GetWorld()->GetTimerManager().ClearTimer(OpacityHandle);
	}
}
