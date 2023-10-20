#include "Utils/DebugUtils.h"

void UDebugUtils::DebugSphere(const UWorld* World, const FVector& Center, FColor Color, float Radius, int32 Segments,
                              bool bPersistent, float LifeTime)
{
	DrawDebugSphere(
		World,
		Center,
		Radius,
		Segments,
		Color,
		bPersistent,
		LifeTime);
}

void UDebugUtils::DebugLine(const UWorld* World, const FVector& Start, const FVector& End, FColor Color,
                            bool bPersistent, float LifeTime, uint8 DepthPriority, float Thickness)
{
	DrawDebugLine(
		World,
		Start,
		End,
		Color,
		bPersistent, LifeTime, DepthPriority,
		Thickness
	);
}
