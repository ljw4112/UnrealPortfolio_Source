#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

class AMainCharacter;
class AFoliage;
class ABuildingBase;
class FDataTables;
class AEnemySpawner;
class UMainHud;
class UDataTable;

USTRUCT()
struct FBuildingInstances
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TArray<ABuildingBase*> BuildingInstances;
};

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CommonWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMainHud* MainHud;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 Hour;

	virtual void Init() override;
	virtual void OnStart() override;

	// Table Initialize
	FDataTables* TableInstance; 
	void InitializeTable();

	// Spawner
	void SetEnemySpawner(AEnemySpawner* SpawnerInstance);
	void SpawnEnemy(bool bTrue = true);
	void KillEnemy(bool bTrue = true);

	// Building
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFoliage> FoliageClass;
	TSubclassOf<ABuildingBase> GetBuildingClass(const FName& BuildingName);
	void AddBuildingInstance(const FName& Key, ABuildingBase* Building);

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, UStaticMesh*> FoliageMesh;

	UPROPERTY()
	TArray<AFoliage*> CreatedFoliage;

	AMainCharacter* GetPlayer();
	
private:
	UPROPERTY()
	AMainCharacter* MainCharacter;
	
	// DataTable
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* TimeEventDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* BuildingDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* ForgeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* ForgeMaterialDataTable;

	// Spawner
	UPROPERTY(VisibleAnywhere)
	AEnemySpawner* EnemySpawner;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, TSubclassOf<ABuildingBase>> BuildingBPData;

	UPROPERTY(VisibleANywhere)
	TMap<FName, FBuildingInstances> BuildingInstance;
};
