#include "Items/BaseItem.h"

#include "AttComponents/Inventory.h"
#include "Character/MainCharacter.h"
#include "Buildings/Foliage.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextBlock.h"
#include "Components/BoxComponent.h"
#include "GameInstance/MainGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/CommonUtils.h"
#include "Widget/InteractWidget.h"
#include "Widget/MainHud.h"
#include "Portfolio01/Consts.h"
#include "Portfolio01/DataTableInstance.h"
#include "Utils/DebugUtils.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetGenerateOverlapEvents(true);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshComp->SetupAttachment(GetRootComponent());
	SkeletalMeshComp->SetRenderCustomDepth(false);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComp->SetupAttachment(GetRootComponent());
	SkeletalMeshComp->SetRenderCustomDepth(false);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(GetRootComponent());

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	if (UCommonUtils::GetCurGameInstance(GetWorld())->TableInstance->ItemData.Contains(ItemName))
	{
		ItemData = UCommonUtils::GetCurGameInstance(GetWorld())->TableInstance->ItemData[ItemName];
	}

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnBoxOverlap);
}

void ABaseItem::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PRINT_SCR("Overlap event started");
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.Add(Actor);
	}
	
	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this, Start, End, FVector(5.f, 5.f, 5.f), BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, BoxHit, true);
	
	if (BoxHit.bBlockingHit)
	{
		PRINT_SCR("Something hitted to axe");
		if (IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor()))
		{
			FHitInformation Information;
			Information.AttackedActor = GetOwner();
			Information.HitLocation = BoxHit.Location;
			
			HitInterface->GetHit(&Information);
		}
	}
}

void ABaseItem::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	SkeletalMeshComp->AttachToComponent(InParent, TransformRules, InSocketName);
}

void ABaseItem::Equip(USceneComponent* InParent, FName InSocketName, AActor* EquipOwner)
{
	if (UInventory* Inventory = UCommonUtils::GetCurGameInstance(GetWorld())->GetPlayer()->GetInventory())
	{
		Inventory->InputItem(ItemName, 1);
	}
	
	if (GetItemType() == EItemType::ETC)
	{
		Destroy();
		return;
	}
	
	if (GetItemType() != EItemType::Weapon && GetItemType() != EItemType::Sweep) return;
	
	AttachMeshToSocket(InParent, InSocketName);
	SetOwner(EquipOwner);
	SkeletalMeshComp->SetGenerateOverlapEvents(false);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SkeletalMeshComp->SetRenderCustomDepth(false);
	UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->InteractWidget->TextInteractive->SetVisibility(ESlateVisibility::Hidden);
}

void ABaseItem::GetInteractive()
{
	if (GetOwner()) return;
	
	SkeletalMeshComp->SetRenderCustomDepth(true);
	UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->InteractWidget->FadeInText();
}

void ABaseItem::GetUnInteractive()
{
	if (GetOwner()) return;
	
	SkeletalMeshComp->SetRenderCustomDepth(false);
	UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->InteractWidget->FadeOutText();
}

void ABaseItem::SetWeaponCollision(bool bEnable)
{
	if (bEnable)
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BoxComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
	{
		
	}
}
