#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactive.h"
#include "Portfolio01/Consts.h"
#include "Portfolio01/DataTable/DataTables.h"
#include "BaseItem.generated.h"

class UBoxComponent;
enum class EItemType : uint8;
class USphereComponent;

UCLASS()
class PORTFOLIO01_API ABaseItem : public AActor, public IInteract
{
	GENERATED_BODY()

public:
	FItemData* ItemData;
	
	ABaseItem();
	virtual void Tick(float DeltaTime) override;

	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* EquipOwner);
	FORCEINLINE USkeletalMeshComponent* GetMesh() { return SkeletalMeshComp; }

	virtual void GetInteractive() override;
	virtual void GetUnInteractive() override;
	
	void SetWeaponCollision(bool bEnable);
	
private:
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* BoxTraceStart;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditDefaultsOnly)
	FName ItemName;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	
protected:
	virtual void BeginPlay() override;

	//void OnOverlapSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FORCEINLINE EItemType GetItemType() { if(ItemData) return static_cast<EItemType>(ItemData->ItemType); return EItemType::None; }
};
