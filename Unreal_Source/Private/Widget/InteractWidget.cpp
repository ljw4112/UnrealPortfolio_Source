#include "Widget/InteractWidget.h"
#include "Animation/WidgetAnimation.h"

void UInteractWidget::FadeInText()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}

void UInteractWidget::FadeOutText()
{
	if (FadeOut)
	{
		PlayAnimation(FadeOut);
	}
}
