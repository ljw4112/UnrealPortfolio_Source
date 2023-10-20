#pragma once
#include "DataTable/DataTables.h"

class FDataTables
{
public:
	TMap<FName, FEnemyData*> EnemyData;
	TMap<FName, FTimeEventData*> TimeEventData;
	TMap<FName, FBuildingData*> BuildingData;
	TMap<FName, FItemData*> ItemData;
	TMap<FName, FForgeData*> ForgeData;
	TMap<FName, FForgeMaterialData*> ForgeMaterialData;
};