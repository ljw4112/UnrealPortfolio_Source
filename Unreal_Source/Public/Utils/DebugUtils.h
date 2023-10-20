#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DebugUtils.generated.h"

#define PRINT_SCR_FORMAT(Format, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT(Format), ##__VA_ARGS__));
#define PRINT_LOG_FORMAT(Format, ...) UE_LOG(LogTemp, Warning, TEXT(Format), ##__VA_ARGS__)
#define PRINT_FORMAT(Format, ...) PRINT_SCR_FORMAT(Format, ...) PRINT_LOG_FORMAT(Format, ...)

#define PRINT_SCR(Text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT(Text)));
#define PRINT_LOG(Text) UE_LOG(LogTemp, Warning, TEXT(Text))
#define PRINT(Text) {PRINT_SCR_FORMAT(Text) PRINT_LOG_FORMAT(Text)}

UCLASS()
class PORTFOLIO01_API UDebugUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void DebugSphere(const UWorld* World, const FVector& Center, FColor Color = FColor::Red, float Radius = 16.f,
	                        int32 Segments = 12,
	                        bool bPersistent = false, float LifeTime = 1.f);

	static void DebugLine(const UWorld* World, const FVector& Start, const FVector& End, FColor Color = FColor::Red,
	                      bool bPersistent = false, float LifeTime = 1.f, uint8 DepthPriority = 0,
	                      float Thickness = 12.f);
};
