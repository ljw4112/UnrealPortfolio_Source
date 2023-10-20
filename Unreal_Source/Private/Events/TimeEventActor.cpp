#include "Events/TimeEventActor.h"

#include "GameInstance/MainGameInstance.h"
#include "Portfolio01/Consts.h"
#include "Portfolio01/DataTableInstance.h"
#include "Utils/CommonUtils.h"
#include "Utils/DebugUtils.h"

ATimeEventActor::ATimeEventActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATimeEventActor::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& Pair : UCommonUtils::GetCurGameInstance(GetWorld())->TableInstance->TimeEventData)
	{
		switch (Pair.Value->EventIndex)
		{
		case ETimeEventIndex::SpawnEnemy:
			{
				TimeEventProof.Add(Pair.Value->ExecTime, ETimeEventIndex::SpawnEnemy);
				break;
			}
		case ETimeEventIndex::KillEnemey:
			{
				TimeEventProof.Add(Pair.Value->ExecTime, ETimeEventIndex::KillEnemey);
				break;
			}

		default:
			break;
		}
	}

	TimeEventFunc.Add(ETimeEventIndex::SpawnEnemy, [this](bool bTrue)
	{
		UCommonUtils::GetCurGameInstance(GetWorld())->SpawnEnemy();
	});

	TimeEventFunc.Add(ETimeEventIndex::KillEnemey, [this](bool bTrue)
	{
		UCommonUtils::GetCurGameInstance(GetWorld())->KillEnemy();
	});

	GetWorld()->GetTimerManager().SetTimer(TickPerSecondHandle, this, &ATimeEventActor::TickPerSecond, TimeEventTickSeconds, true);
}

void ATimeEventActor::TickPerSecond()
{
	if (!GetWorld()) return;

	if (int32 CurGameHour = UCommonUtils::GetCurGameInstance(GetWorld())->Hour;
		TimeEventProof.Contains(CurGameHour))
	{
		TimeEventFunc[TimeEventProof[CurGameHour]](true);
	}
}
