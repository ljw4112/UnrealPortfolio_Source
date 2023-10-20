#pragma once

#include "Engine/DataTable.h"
#include "DataTables.generated.h"

USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FEnemyData()
	:Name(FName(""))
	,MaxHP(0)
	,WalkSpeed(0)
	,RunSpeed(0)
	,AttackPower(0)
	,Blueprint(FName("")){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Blueprint;
};

USTRUCT(BlueprintType)
struct FTimeEventData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FTimeEventData()
	:EventIndex(0)
	,ExecTime(0.f){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EventIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExecTime;
};

USTRUCT(BlueprintType)
struct FBuildingData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBuildingData()
    :MeshName(FName(""))
    ,BuildingRecipe(0){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MeshName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuildingRecipe;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FItemData()
	:bCanHaveMulti(true),ItemType(0), Thumbnail(FName()){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanHaveMulti;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Thumbnail;
};

USTRUCT(BlueprintType)
struct FForgeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FForgeData():Materials(FName("")){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Materials;
};

USTRUCT(BlueprintType)
struct FForgeMaterialData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FForgeMaterialData():ItemName(FName("")), NeedCount(0){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NeedCount;
};