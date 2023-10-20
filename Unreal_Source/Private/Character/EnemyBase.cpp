#include "Character/EnemyBase.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "NavigationInvokerComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "AttComponents/CharacterAttribute.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Portfolio01/Consts.h"
#include "Utils/CommonUtils.h"
#include "Interfaces/HitInterface.h"
#include "Utils/DebugUtils.h"
#include "Components/WidgetComponent.h"
#include "Widget/EnemyHPWidget.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/UIUtils.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("Navigation Invoker"));

	AttackBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Box Component"));
	AttackBoxComponent->SetupAttachment(GetRootComponent());
	AttackBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackBoxComponent->SetGenerateOverlapEvents(true);

	EnemyAttribute = CreateDefaultSubobject<UCharacterAttribute>(TEXT("Enemy Attribute"));

	EnemyHPBarComponents = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	EnemyHPBarComponents->SetupAttachment(GetRootComponent());
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	EnemyMoveStatus = EEnemyMoveStatus::Idle;
	AttackBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::AttackOverlapBegin);

	EnemyData = DataTable->FindRow<FEnemyData>(RowName, "");
	if (!EnemyData)
	{
		PRINT_SCR("NO Enemy Data here!");
		return;
	}

	EnemyAttribute->InitializeData(EnemyData);
	EnemyHPWidget = Cast<UEnemyHPWidget>(EnemyHPBarComponents->GetWidget());
	Tags.Add(FTag::Enemy);
}

void AEnemyBase::ChangeWalkSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AEnemyBase::ChangeMoveStatus(EEnemyMoveStatus MoveStatus)
{
	EnemyMoveStatus = MoveStatus;
}

void AEnemyBase::EnableAttackCollision()
{
	SetAttackCollision(true);
}

void AEnemyBase::DisableAttackCollision()
{
	SetAttackCollision(false);
}

float AEnemyBase::Attack()
{
	if (!AttackMontage && UsableAttackSections.Num() <= 0) return 0.f;

	int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, UsableAttackSections.Num() - 1);
	return UCommonUtils::PlayMontage(GetMesh()->GetAnimInstance(), AttackMontage, UsableAttackSections[RandomIndex]);
}

void AEnemyBase::Die()
{
	EnemyMoveStatus = EEnemyMoveStatus::Dead;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetAttackCollision(false);
	EnemyAttribute->SetHP(EnemyAttribute->GetCurrentHP());

	UAIBlueprintHelperLibrary::GetAIController(this)->BrainComponent->StopLogic(FString("Dead"));
	UCommonUtils::PlayMontage(GetMesh()->GetAnimInstance(), DieMontage);

	UUIUtils::SetActive(EnemyHPWidget, false);
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemyBase::DestroyActor, DestroyDelayTime, false);
}

void AEnemyBase::SetAttackCollision(bool bActive)
{
	if (bActive)
		AttackBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		AttackBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyBase::AttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor))
	{
		FHitInformation* Information = new FHitInformation();
		Information->AttackedActor = this;
		
		HitInterface->GetHit(Information);
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (EventInstigator)
	{
		EnemyAttribute->SetHP(DamageAmount);
		EnemyHPWidget->SetPercent(EnemyAttribute->GetHPPercent());
		AttackedActor = EventInstigator->GetCharacter();

		if (EnemyAttribute->GetCurrentHP() <= 0)
		{
			Die();
		}
	}

	return EnemyAttribute->GetCurrentHP();
}

void AEnemyBase::GetHit(const FHitInformation* HitInformation)
{
	if (HitInformation->AttackedActor->Tags.Contains(FName("Turret")))
	{
		if (TurretBulletParticle)
		{
			UCommonUtils::PlayMontage(GetMesh()->GetAnimInstance(), HitReactMontage);
			UCommonUtils::PlayParticle(GetWorld(), TurretBulletParticle, GetActorLocation());
		}
	}
	else
	{
		bIsAttacked = true;
		float Timer = UCommonUtils::PlayMontage(GetMesh()->GetAnimInstance(), HitReactMontage);
		GetWorldTimerManager().SetTimer(ChaseTimer, this, &AEnemyBase::ChaseTarget, Timer, false);
	}
}

void AEnemyBase::ChaseTarget()
{
	bIsAttacked = false;
	GetWorldTimerManager().ClearTimer(ChaseTimer);
}

void AEnemyBase::DestroyActor()
{
	Destroy();
}

FEnemyData AEnemyBase::GetEnemyData()
{
	return *EnemyData;
}

