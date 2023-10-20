#pragma once

class ABaseItem;

UENUM(BlueprintType)
enum class EEnemyMoveStatus : uint8
{
	Idle = 0,
	Walk = 1,
	Run = 2,
	Dead = 3
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Dead = 0,
	Idle = 1,
	WeaponIdle = 2,
	WeaponAim = 3,
	Sweep = 4
};

UENUM(BlueprintType)
enum class ECameraState : uint8
{
	Idle = 0,
	Zoom = 1,
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0,
	Weapon = 1,
	Sweep = 2,
	ETC = 3
};

UENUM(BlueprintType)
enum class ETimeEventIndex
{
	None = 0,
	SpawnEnemy = 1,
	KillEnemey = 2,
};

class FSocketName
{
public:
	inline static FName RifleEquipSocket = TEXT("hand_rSocket_Rifle");
	inline static FName MuzzleSocket = TEXT("Muzzle");
	inline static FName TurretFireSocket = TEXT("FireSocket");
	inline static FName SweepSocket = TEXT("hand_rSocket_Sweep");
};

class FSectionName
{
public:
	// Player
	inline static FName IdleToAim = TEXT("IdleToAim");
	inline static FName Fire = TEXT("Fire");
	inline static FName AimToIdle = TEXT("AimToIdle");

	// Enemy
	inline static FName Punch = TEXT("Punch");
	inline static FName Head = TEXT("Head");
	inline static FName Attack = TEXT("Attack");
};

class FTag
{
public:
	inline static FName Player = TEXT("Player");
	inline static FName Enemy = TEXT("Enemy");
};

class FHitInformation
{
public:
	AActor* AttackedActor;
	FVector HitLocation;
};
