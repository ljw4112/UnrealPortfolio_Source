#include "Buildings/Foliage.h"

#include "Components/BoxComponent.h"
#include "Utils/DebugUtils.h"


// Sets default values
AFoliage::AFoliage()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(StaticMesh);
	
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetCollisionObjectType(ECC_WorldDynamic);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void AFoliage::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFoliage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFoliage::SetStaticMesh(UStaticMesh* Mesh)
{
	if (Mesh)
	{
		StaticMesh->SetStaticMesh(Mesh);
	}
}

void AFoliage::SetChop()
{
	// CurChopCnt++;
	// if (MaxChopCnt == CurChopCnt)
	// {
	// 	StaticMesh->SetSimulatePhysics(true);
	// 	StaticMesh->SetMassOverrideInKg(NAME_None, 2000.f, true);
	// 	return;
	// }

	PRINT_SCR_FORMAT("Left Chop Count : %d", MaxChopCnt - CurChopCnt);
}

void AFoliage::GetHit(const FHitInformation* HitInformation)
{
	IHitInterface::GetHit(HitInformation);
	SetChop();
}

