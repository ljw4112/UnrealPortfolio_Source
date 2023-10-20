#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BuildingBase.generated.h"

class UArrowComponent;
class USphereComponent;

UCLASS()
class PORTFOLIO01_API ABuildingBase : public APawn
{
	GENERATED_BODY()

public:
	ABuildingBase();
	virtual void Tick(float DeltaTime) override;
	float GetHeight();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* ArrowComponent;
};
