#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyBase;
class ATargetPoint;

UCLASS()
class PORTFOLIO01_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy();
	void KillEnemy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AEnemyBase>> EnemyClasses;

	UPROPERTY(EditAnywhere)
	float SpawnSphereRadius = 0.f;

	UPROPERTY(EditAnywhere)
	bool bDebugSpawnableArea = false;

	UPROPERTY(EditAnywhere)
	int32 MaxEnemyCount;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentEnemyCount;

private:
	TArray<AEnemyBase*> EnemyContainer;
};
