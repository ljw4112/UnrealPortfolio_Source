#include "AttComponents/BuildSystem.h"

#include "Buildings/BuildingBase.h"
#include "Camera/CameraComponent.h"
#include "Character/MainCharacter.h"
#include "GameInstance/MainGameInstance.h"
#include "Utils/CommonUtils.h"
#include "Utils/DebugUtils.h"

UBuildSystem::UBuildSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuildSystem::BeginPlay()
{
	Super::BeginPlay();

	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetOwner()))
	{
		this->Character = MainCharacter;
		CameraComponent = MainCharacter->GetCameraComponent();
	}
}

void UBuildSystem::LoopPreview()
{
	if (!Character)
	{
		PRINT_SCR("Character is null!!");
		return;
	}

	FTransform Transform = Character->GetCenterTransform();
	FVector StartPosition = Transform.GetLocation();
	FVector ForwardVector = Character->GetCameraComponent()->GetComponentTransform().GetRotation().GetForwardVector() *
		700.f;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Character);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartPosition, StartPosition + ForwardVector, ECC_Visibility,
	                                     CollisionParams);
	if (HitResult.bBlockingHit)
	{
		bCanBuild = true;
		FTransform BuildPreviewTransform;
		if (!PreviewMesh && BuildingMesh && PreviewMaterial)
		{
			PreviewMesh = Cast<UStaticMeshComponent>(
				Character->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, BuildPreviewTransform,
				                               false));
			PreviewMesh->SetStaticMesh(BuildingMesh);
			PreviewMesh->SetMaterial(0, PreviewMaterial);
		}

		if (PreviewMesh && BuildingMesh)
		{
			PreviewMesh->SetVisibility(true);
			HitResult.ImpactPoint.Z += BuildingMesh->GetBoundingBox().GetSize().Z;

			BuildPreviewTransform.SetLocation(HitResult.ImpactPoint);
			PreviewMesh->SetWorldTransform(BuildPreviewTransform);
		}
	}
	else
	{
		bCanBuild = false;

		if (PreviewMesh && Character)
		{
			PreviewMesh->SetVisibility(false);
		}
	}
}


void UBuildSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bBuildMode)
	{
		LoopPreview();
	}
	else
	{
		// 건물을 쥐고 있고 땅에 닿여있는 상태여서 건설 가능한 경우
		if (bCanBuild)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (TSubclassOf<ABuildingBase> BuildingPawn = UCommonUtils::GetCurGameInstance(GetWorld())->GetBuildingClass(
				*BuildingMesh->GetName()))
			{
				if (ABuildingBase* Building = Cast<ABuildingBase>(GetWorld()->SpawnActor<AActor>(BuildingPawn, PreviewMesh->GetComponentLocation(),
				                                                PreviewMesh->GetComponentRotation(), SpawnParameters)))
				{
					UCommonUtils::GetCurGameInstance(GetWorld())->AddBuildingInstance(*BuildingMesh->GetName(), Building);

					FVector UpperLocation = Building->GetActorLocation();
					UpperLocation.Z += Building->GetHeight();

					Building->SetActorLocation(UpperLocation);
				}
			}
		}

		if (Character && PreviewMesh)
		{
			PreviewMesh->SetStaticMesh(nullptr);
			PreviewMesh->SetMaterial(0, nullptr);
			Character->RemoveOwnedComponent(PreviewMesh);
			PreviewMesh = nullptr;
		}

		bCanBuild = false;
	}
}

void UBuildSystem::SetBuildMode(bool bOn)
{
	bBuildMode = bOn;
}

bool UBuildSystem::GetBuildMode()
{
	return bBuildMode;
}

bool UBuildSystem::GetCanBuild()
{
	return bCanBuild;
}
