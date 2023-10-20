#include "GameInstance/MainGameInstance.h"

#include "Character/MainCharacter.h"
#include "GameActors/EnemySpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/MainHud.h"
#include "Portfolio01/DataTableInstance.h"
#include "Utils/DebugUtils.h"

void UMainGameInstance::Init()
{
	Super::Init();
}

void UMainGameInstance::OnStart()
{
	Super::OnStart();

	MainHud = CreateWidget<UMainHud>(GetWorld(), CommonWidget);
	if (MainHud)
	{
		MainHud->AddToViewport();
		MainHud->InitializeUI();
	}

	InitializeTable();
}

void UMainGameInstance::InitializeTable()
{
	TableInstance = new FDataTables();

	if (EnemyDataTable)
	{
		//1. Enemy Data Table
		for (auto It : EnemyDataTable->GetRowMap())
		{
			FEnemyData* Data = reinterpret_cast<FEnemyData*>(It.Value);
			TableInstance->EnemyData.Add(FName(Data->Name), Data);
		}
	}

	if (TimeEventDataTable)
	{
		//2. Time Event Data Table
        for (auto It : TimeEventDataTable->GetRowMap())
        {
        	FTimeEventData* Data = reinterpret_cast<FTimeEventData*>(It.Value);
        	TableInstance->TimeEventData.Add(FName(FString::FromInt(Data->EventIndex)), Data);
        }
	}	

	if (BuildingDataTable)
	{
		for (auto It : BuildingDataTable->GetRowMap())
		{
			FBuildingData* Data = reinterpret_cast<FBuildingData*>(It.Value);
			TableInstance->BuildingData.Add(It.Key, Data);
		}
	}

	if (ItemDataTable)
	{
		for (auto It : ItemDataTable->GetRowMap())
		{
			FItemData* Data = reinterpret_cast<FItemData*>(It.Value);
			TableInstance->ItemData.Add(It.Key, Data);
		}
	}

	if (ForgeDataTable)
	{
		for (auto It : ForgeDataTable->GetRowMap())
		{
			FForgeData* Data = reinterpret_cast<FForgeData*>(It.Value);
			TableInstance->ForgeData.Add(It.Key, Data);
		}
	}

	if (ForgeMaterialDataTable)
	{
		for (auto It : ForgeMaterialDataTable->GetRowMap())
		{
			FForgeMaterialData* Data = reinterpret_cast<FForgeMaterialData*>(It.Value);
			TableInstance->ForgeMaterialData.Add(It.Key, Data);
		}
	}

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UMainGameInstance::SetEnemySpawner(AEnemySpawner* SpawnerInstance)
{
	EnemySpawner = SpawnerInstance;
	PRINT_SCR("Enemy Spawner is registered on Game Instance");
}

void UMainGameInstance::SpawnEnemy(bool bTrue)
{
	EnemySpawner->SpawnEnemy();
	GetPlayer()->ChangeAmbientSound(1);
}

void UMainGameInstance::KillEnemy(bool bTrue)
{
	EnemySpawner->KillEnemy();
	GetPlayer()->ChangeAmbientSound(0);
}

TSubclassOf<ABuildingBase> UMainGameInstance::GetBuildingClass(const FName& BuildingName)
{
	if (BuildingName.IsNone()) return nullptr;
	if (!BuildingBPData.Contains(BuildingName)) return nullptr;

	return BuildingBPData[BuildingName];
}

void UMainGameInstance::AddBuildingInstance(const FName& Key, ABuildingBase* Building)
{
	if (!BuildingInstance.Contains(Key))
	{
		FBuildingInstances Instance;
		BuildingInstance.Add(Key, Instance);	
	}
	BuildingInstance[Key].BuildingInstances.Add(Building);
}

AMainCharacter* UMainGameInstance::GetPlayer()
{
	if (!MainCharacter)
	{
		MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	return MainCharacter;
}
