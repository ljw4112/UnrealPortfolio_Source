#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UTextBlock;

UCLASS()
class PORTFOLIO01_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextInteractive;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeIn;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOut;

	void FadeInText();
	void FadeOutText();
};
