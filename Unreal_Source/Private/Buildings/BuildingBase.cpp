#include "Buildings/BuildingBase.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"


ABuildingBase::ABuildingBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(SphereComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(SphereComponent);
}

void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ABuildingBase::GetHeight()
{
	return SphereComponent->GetScaledSphereRadius() / 2;
}

