#include "Buildings/Turret.h"

#include "Buildings/TurretBullet.h"
#include "Character/EnemyBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "Portfolio01/Consts.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;


	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HEAD"));
	TurretHead->SetupAttachment(SphereComponent);
	
	TurretBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	TurretBody->SetupAttachment(SphereComponent);
	
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	PawnSensing->bOnlySensePlayers = false;
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &ATurret::OnPawnSensing);
}

void ATurret::OnPawnSensing(APawn* Pawn)
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(Pawn);
	if (!Enemy || !TurretHead) return;

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Enemy->GetActorLocation()));

	if (!GetWorldTimerManager().IsTimerActive(TurretFireHandle))
	{
		GetWorldTimerManager().SetTimer(TurretFireHandle, this, &ATurret::FireBullet, FireDelay, true);
	}
}

void ATurret::FireBullet()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FVector FireLocation = TurretHead->GetSocketLocation(FSocketName::TurretFireSocket);
	
	ATurretBullet* Bullet = GetWorld()->SpawnActor<ATurretBullet>(TurretBulletClass, FireLocation, GetActorRotation(), SpawnParameters);
	Bullet->SetLifeSpan(5.f);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
