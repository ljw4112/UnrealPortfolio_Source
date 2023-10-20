#include "GameActors/EnemySpawner.h"

#include "GameInstance/MainGameInstance.h"
#include "Engine/TargetPoint.h"
#include "Character/EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/CommonUtils.h"
#include "Utils/DebugUtils.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	PRINT_SCR("Enemy Spawner is created");

	UCommonUtils::GetCurGameInstance(GetWorld())->SetEnemySpawner(this);
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDebugSpawnableArea)
	{
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation(), SpawnSphereRadius);
	}
}

void AEnemySpawner::SpawnEnemy()
{
	if (EnemyClasses.Num() <= 0) return;
	if (CurrentEnemyCount >= MaxEnemyCount)
	{
		PRINT_SCR("Enemy is full stack!");
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	while (CurrentEnemyCount < MaxEnemyCount)
	{
		FNavLocation SpawnPosition;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), SpawnSphereRadius, SpawnPosition);
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), SpawnPosition.Location, 50.f, 12, FLinearColor::Red);

		int32 RandomEnemy = UKismetMathLibrary::RandomInteger(EnemyClasses.Num() - 1);
		if (AEnemyBase* Enemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyClasses[RandomEnemy], SpawnPosition.Location,
																   GetActorRotation(), SpawnParameters))
		{
			EnemyContainer.Add(Enemy);
			CurrentEnemyCount++;
			PRINT_SCR("Enemy is created successfully.");
		}
	}
}

void AEnemySpawner::KillEnemy()
{
	PRINT_SCR("KillEnemy function called");

	for (int i = 0; i < EnemyContainer.Num(); i++)
	{
		if (AEnemyBase* Enemy = EnemyContainer[i])
		{
			Enemy->Die();
			EnemyContainer.RemoveAt(i--);
		}
	}
}
