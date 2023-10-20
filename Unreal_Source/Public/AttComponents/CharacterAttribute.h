#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttribute.generated.h"

struct FEnemyData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PORTFOLIO01_API UCharacterAttribute : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterAttribute();
	void InitializeData(const FEnemyData* EnemyData);
	void SetHP(int32 Damage);

private:
	UPROPERTY(VisibleDefaultsOnly)
	float MaxHP = 0;

	UPROPERTY(EditDefaultsOnly)
	float CurrentHP = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttackPower = 0;

public:
	FORCEINLINE int32 GetCurrentHP() { return CurrentHP; }
	FORCEINLINE int32 GetAttackPower() { return AttackPower; }
	FORCEINLINE float GetHPPercent() { return CurrentHP / MaxHP; }
};
