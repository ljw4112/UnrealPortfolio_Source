#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BloodScreen.generated.h"

class UImage;

UCLASS()
class PORTFOLIO01_API UBloodScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UImage* BloodImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GetHitAnim;

	void TurnOnBloodImage();
};
