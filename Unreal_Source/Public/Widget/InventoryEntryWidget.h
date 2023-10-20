#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "InventoryEntryWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class PORTFOLIO01_API UInventoryEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemCount;

	// Constructor
	virtual void NativeConstruct() override;
	
	// Called when Item UI Settings
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	// Called when Item selected
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
};
