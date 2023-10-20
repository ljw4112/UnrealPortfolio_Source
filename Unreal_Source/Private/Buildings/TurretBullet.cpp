#include "Buildings/TurretBullet.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/HitInterface.h"
#include "Portfolio01/Consts.h"

ATurretBullet::ATurretBullet()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Mesh"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 3000.f;

	Tags.Add(FName("Turret"));
}

void ATurretBullet::BeginPlay()
{
	Super::BeginPlay();

	BulletMesh->OnComponentBeginOverlap.AddDynamic(this, &ATurretBullet::OnBulletOverlap);
}

void ATurretBullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IHitInterface* Interface = Cast<IHitInterface>(OtherActor);
	if (!Interface) return;

	FHitInformation* HitInformation = new FHitInformation();
	HitInformation->AttackedActor = this;
	
	Interface->GetHit(HitInformation);
	Destroy();
}

void ATurretBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
