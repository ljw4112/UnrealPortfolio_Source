#include "Widget/MainHud.h"

#include "Components/CanvasPanel.h"
#include "Utils/UIUtils.h"
#include "Widget/BloodScreen.h"
#include "Widget/ForgeWidget.h"
#include "Widget/InteractWidget.h"
#include "Widget/InventoryWidget.h"

void UMainHud::InitializeUI()
{
	SetVisibility(ESlateVisibility::Visible);
	
	// Crosshair->AddToViewport();
	// Interact->AddToViewport();
	// Inventory->AddToViewport();
	// BloodScreen->AddToViewport();
	// Forge->AddToViewport();

	UUIUtils::SetActive(Crosshair, false);
	UUIUtils::SetActive(Inventory, false);
	UUIUtils::SetActive(Forge, false);

	InteractWidget = Cast<UInteractWidget>(Interact);
	InventoryWidget = Cast<UInventoryWidget>(Inventory);
	BloodScreenWidget = Cast<UBloodScreen>(BloodScreen);
	ForgeWidget = Cast<UForgeWidget>(Forge);
}
