#include "Widget/ForgeWidget.h"

#include "AttComponents/Inventory.h"
#include "Character/MainCharacter.h"
#include "Utils/CommonUtils.h"
#include "GameInstance/MainGameInstance.h"

int32 UForgeWidget::GetItemCount(FName ItemName)
{
	return UCommonUtils::GetCurGameInstance(GetWorld())->GetPlayer()->GetInventory()->GetItemCount(ItemName);
}

bool UForgeWidget::OnClose()
{
	return OnCloseFunc.ExecuteIfBound();
}
