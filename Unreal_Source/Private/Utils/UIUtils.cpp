#include "Utils/UIUtils.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Widget.h"

FVector2D UUIUtils::GetUIPosition(const UWidget* Widget)
{
	if (!Widget) return FVector2D();

	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	if (!CanvasPanelSlot) return FVector2D();

	return CanvasPanelSlot->GetPosition();
}

bool UUIUtils::ChangeUIPosition(const UWidget* Widget, const FVector2D Position)
{
	if (!Widget) return false;

	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Widget->Slot);

	if (!CanvasPanelSlot) return false;
	CanvasPanelSlot->SetPosition(Position);

	return true;
}

bool UUIUtils::SetActive(UWidget* Widget, bool IsActive)
{
	if (!Widget) return false;

	if (IsActive)
	{
		Widget->SetVisibility(ESlateVisibility::Visible);
		return true;
	}
	else
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
		return true;
	}
}

bool UUIUtils::GetActive(const UWidget* Widget)
{
	switch (Widget->GetVisibility())
	{
	case ESlateVisibility::Collapsed:
	case ESlateVisibility::Hidden:
	case ESlateVisibility::HitTestInvisible:
		return false;
	case ESlateVisibility::Visible:
	case ESlateVisibility::SelfHitTestInvisible:
		return true;
	default:
		return false;
	}
}
