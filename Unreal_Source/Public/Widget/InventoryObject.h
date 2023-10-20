#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryObject.generated.h"

UCLASS()
class PORTFOLIO01_API UInventoryObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName IconName;

	UPROPERTY(EditAnywhere)
	int32 ItemCount;
};
