#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIUtils.generated.h"

class UWidget;

UCLASS()
class PORTFOLIO01_API UUIUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FVector2D GetUIPosition(const UWidget* Widget);
	static bool ChangeUIPosition(const UWidget* Widget, const FVector2D Position);
	static bool SetActive(UWidget* Widget, bool IsActive);
	static bool GetActive(const UWidget* Widget);
};
