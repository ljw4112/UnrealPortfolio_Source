#include "Widget/InventoryWidget.h"

#include "AttComponents/Inventory.h"
#include "Character/MainCharacter.h"
#include "Components/TileView.h"
#include "GameInstance/MainGameInstance.h"
#include "Items/BaseItem.h"
#include "Portfolio01/DataTableInstance.h"
#include "Utils/CommonUtils.h"
#include "Widget/InventoryObject.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::RefreshListBox()
{
	if (!Inventory)
	{
		Inventory = UCommonUtils::GetCurGameInstance(GetWorld())->GetPlayer()->GetInventory();
	}
	auto InvenInstance = Inventory->GetInventoryInstance();
	for (auto Item : InvenInstance)
	{
		if (UInventoryObject* InvenObject = NewObject<UInventoryObject>(UInventoryObject::StaticClass()))
		{
			FItemData* ItemData = UCommonUtils::GetCurGameInstance(GetWorld())->TableInstance->ItemData[Item.Key];
			if (!ItemData) continue;
			
			InvenObject->IconName = ItemData->Thumbnail;
			InvenObject->ItemCount = Item.Value.ItemCount;

			TileView->AddItem(InvenObject);
		}
	}
}

void UInventoryWidget::RemoveAllItems()
{
	TileView->ClearListItems();
}
