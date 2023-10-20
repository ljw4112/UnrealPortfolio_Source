#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventory;
class AMainCharacter;
class UTileView;
/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTileView* TileView;

	virtual void NativeConstruct() override;

	// Call when Item panel open
	void RefreshListBox();

	void RemoveAllItems();

private:
	UPROPERTY()
	AMainCharacter* MainCharacter;

	UPROPERTY()
	UInventory* Inventory;
};
