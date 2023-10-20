#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Portfolio01/DataTable/DataTables.h"
#include "EnemyBase.generated.h"

class FHitInformation;
class UEnemyHPWidget;
class UWidgetComponent;class UCharacterAttribute;
class UBoxComponent;
enum class EEnemyMoveStatus : uint8;
class UNavigationInvokerComponent;

UCLASS()
class PORTFOLIO01_API AEnemyBase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemyBase();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit(const FHitInformation* HitInformation) override;
	
	// Die
	UFUNCTION(BlueprintCallable)
	void Die();

protected:
	virtual void BeginPlay() override;

	// Data Settings
	UPROPERTY(EditDefaultsOnly)
	UDataTable* DataTable;

	FEnemyData* EnemyData;

	UPROPERTY(EditDefaultsOnly)
	FName RowName;

	UPROPERTY(VisibleAnywhere)
	UCharacterAttribute* EnemyAttribute;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* EnemyHPBarComponents;

	UPROPERTY()
	UEnemyHPWidget* EnemyHPWidget;

	UFUNCTION(BlueprintCallable, Category = Move)
	void ChangeWalkSpeed(float Speed);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Move)
	int32 WalkSpeed = 100;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Move)
	int32 RunSpeed = 300;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Move)
	EEnemyMoveStatus EnemyMoveStatus;

	UFUNCTION(BlueprintCallable)
	void ChangeMoveStatus(EEnemyMoveStatus MoveStatus);

	UFUNCTION(BlueprintCallable)
	void EnableAttackCollision();

	UFUNCTION(BlueprintCallable)
	void DisableAttackCollision();

	UFUNCTION(BlueprintCallable)
	float Attack();

	// Hit
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TurretBulletParticle;

private:
	// Components	
	void SetAttackCollision(bool bActive);
	
	// AI
	UPROPERTY(EditAnywhere, Category = AI)
	UNavigationInvokerComponent* NavigationInvoker;

	// Attack
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* AttackedActor;
	
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	UBoxComponent* AttackBoxComponent;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> UsableAttackSections;

	UFUNCTION()
	void AttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Hit
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactMontage;

	FTimerHandle ChaseTimer;

	void ChaseTarget();

	// Die
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DieMontage;

	UPROPERTY(EditDefaultsOnly)
	float DestroyDelayTime = 3.f;
	
	FTimerHandle DestroyTimer;
	void DestroyActor();

public:
	UFUNCTION(BlueprintCallable)
	FEnemyData GetEnemyData();
};