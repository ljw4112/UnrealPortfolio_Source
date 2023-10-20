#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class ATurretBullet;
class UArrowComponent;
class USphereComponent;
class UPawnSensingComponent;

UCLASS()
class PORTFOLIO01_API ATurret : public ABuildingBase
{
	GENERATED_BODY()

public:
	ATurret();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TurretBody;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TurretHead;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATurretBullet> TurretBulletClass;

	UFUNCTION()
	void OnPawnSensing(APawn* Pawn);

	UPROPERTY(EditAnywhere)
	float FireDelay;

private:
	FTimerHandle TurretFireHandle;
	void FireBullet();
};
