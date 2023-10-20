#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "Foliage.generated.h"

class UBoxComponent;

UCLASS()
class PORTFOLIO01_API AFoliage : public AActor, public IHitInterface
{
	GENERATED_BODY()

public:
	AFoliage();
	virtual void Tick(float DeltaTime) override;

	void SetStaticMesh(UStaticMesh* Mesh);
	void SetChop();

	virtual void GetHit(const FHitInformation* HitInformation) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

private:
	UPROPERTY(VisibleAnywhere)
	int32 CurChopCnt = 0;

	UPROPERTY(VisibleAnywhere)
	int32 MaxChopCnt = 0;
};
