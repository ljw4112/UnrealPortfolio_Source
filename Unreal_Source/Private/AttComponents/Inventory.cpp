#include "AttComponents/Inventory.h"

#include "GameInstance/MainGameInstance.h"
#include "Items/BaseItem.h"
#include "Portfolio01/DataTableInstance.h"
#include "Utils/CommonUtils.h"
#include "Utils/DebugUtils.h"
#include "Widget/InventoryWidget.h"
#include "Widget/MainHud.h"

UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventory::BeginPlay()
{
	Super::BeginPlay();
}

void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventory::InputItem(const FName& ItemName, int32 Count)
{
	if (!HaveItem(ItemName))
	{
		FItemStructure ItemStructure;
		ItemStructure.ItemName = ItemName;
		ItemStructure.ItemCount = Count;

		InventoryInstance.Add(ItemName, ItemStructure);
	}
	else
	{
		InventoryInstance[ItemName].ItemCount += Count;
	}

	PRINT_SCR_FORMAT("Item: %s inputed", *ItemName.ToString())
}

int32 UInventory::GetItemCount(FName ItemName)
{
	if (!HaveItem(ItemName)) return 0;
	return InventoryInstance[ItemName].ItemCount;
}

bool UInventory::HaveItem(FName ItemName)
{
	return InventoryInstance.Contains(ItemName);
}

bool UInventory::UseItem(FName ItemName, int Count)
{
	if (!HaveItem(ItemName) || GetItemCount(ItemName) < Count) return false;
	InventoryInstance[ItemName].ItemCount -= Count;

	PRINT_SCR_FORMAT("%s Item used, Cur Count : %d", *ItemName.ToString(), InventoryInstance[ItemName].ItemCount);

	if (InventoryInstance[ItemName].ItemCount <= 0)
	{
		InventoryInstance.Remove(ItemName);
	}

	return true;
}

bool UInventory::RemoveForgeMaterial(FName ItemName)
{
	UMainGameInstance* GameInstance = UCommonUtils::GetCurGameInstance(GetWorld());
	if (!GameInstance) return false;

	if (!GameInstance->TableInstance->ForgeData.Contains(ItemName)) return false;
	FForgeData* ForgeData = GameInstance->TableInstance->ForgeData[ItemName];

	FString ForgeMaterialName = ForgeData->Materials.ToString();
	if (!GameInstance->TableInstance->ForgeMaterialData.Contains(*ForgeMaterialName)) return false;

	if (FForgeMaterialData* MaterialData = GameInstance->TableInstance->ForgeMaterialData[*ForgeMaterialName]; UseItem(
		MaterialData->ItemName, MaterialData->NeedCount)) return true;
	return false;
}

TMap<FName, FItemStructure> UInventory::GetInventoryInstance()
{
	return InventoryInstance;
}
