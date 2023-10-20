#include "Widget/InventoryEntryWidget.h"

#include "PaperSprite.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateBrush.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Widget/InventoryObject.h"
#include "Utils/CommonUtils.h"

void UInventoryEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (UInventoryObject* InvenObject = Cast<UInventoryObject>(ListItemObject))
	{
		FString Text = FString::FromInt(InvenObject->ItemCount); 
		ItemCount->SetText(FText::FromString(Text));

		if (UPaperSprite* Thumbnail = UCommonUtils::LoadPaperSprite_FromFile(InvenObject->IconName.ToString()))
		{
			FButtonStyle Style;
			FSlateBrush Brush;

			Brush.SetResourceObject(Thumbnail);
			Style.SetNormal(Brush);

			Button->SetStyle(Style);
		}
	}
}

void UInventoryEntryWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}
