#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeEventActor.generated.h"

enum class ETimeEventIndex;

UCLASS()
class PORTFOLIO01_API ATimeEventActor : public AActor
{
	GENERATED_BODY()

public:
	ATimeEventActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float TimeEventTickSeconds = 0.05f;

private:
	TMap<int32, ETimeEventIndex> TimeEventProof;
	TMap<ETimeEventIndex, TFunction<void(bool)>> TimeEventFunc;

	FTimerHandle TickPerSecondHandle;
	void TickPerSecond();
};
