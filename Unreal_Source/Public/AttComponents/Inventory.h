#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

class ABaseItem;

USTRUCT(BlueprintType)
struct FItemStructure
{
	GENERATED_USTRUCT_BODY()

	FItemStructure() : ItemName(FName("")), ItemCount(0) {}

	UPROPERTY(EditAnywhere)
	FName ItemName;;

	UPROPERTY(VisibleAnywhere)
	int32 ItemCount;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PORTFOLIO01_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventory();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;
	
	TMap<FName, FItemStructure> GetInventoryInstance();

	void InputItem(const FName& ItemName, int32 Count = 1);
	int32 GetItemCount(FName ItemName);
	bool HaveItem(FName ItemName);
	bool UseItem(FName ItemName, int Count = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveForgeMaterial(FName ItemName);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FItemStructure> InventoryInstance;
};
