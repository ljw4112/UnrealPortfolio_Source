#include "AttComponents/CharacterAttribute.h"
#include "Portfolio01/DataTable/DataTables.h"
#include "Kismet/KismetMathLibrary.h"

UCharacterAttribute::UCharacterAttribute()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterAttribute::InitializeData(const FEnemyData* EnemyData)
{
	MaxHP = EnemyData->MaxHP;
	AttackPower = EnemyData->AttackPower;

	CurrentHP = MaxHP;
}

void UCharacterAttribute::SetHP(int32 Damage)
{
	CurrentHP = UKismetMathLibrary::Clamp(CurrentHP - Damage, 0, MaxHP);
}
