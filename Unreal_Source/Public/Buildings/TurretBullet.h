#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TurretBullet.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class PORTFOLIO01_API ATurretBullet : public APawn
{
	GENERATED_BODY()

public:
	ATurretBullet();
	virtual void Tick(float DeltaTime) override;
	

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
