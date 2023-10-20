#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildSystem.generated.h"

class AMainCharacter;
class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PORTFOLIO01_API UBuildSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuildSystem();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	void SetBuildMode(bool bOn);
	bool GetBuildMode();
	bool GetCanBuild();

protected:
	virtual void BeginPlay() override;

	void LoopPreview();

	UPROPERTY()
	UCameraComponent* CameraComponent;

	UPROPERTY()
	AMainCharacter* Character;

	UPROPERTY()
	UStaticMeshComponent* PreviewMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* BuildingMesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* PreviewMaterial;

private:
	bool bBuildMode = false;
	bool bCanBuild = false;
};
