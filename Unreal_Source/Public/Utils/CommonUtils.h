// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonUtils.generated.h"

class UPaperSprite;
class UMainGameInstance;
enum class ESubsystem : uint8;
class UAnimInstance;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UCommonUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static float PlayMontage(UAnimInstance* Instance, UAnimMontage* Montage, FName SectionName = FName());
	static UMainGameInstance* GetCurGameInstance(const UWorld* World);
	static void PlayParticle(const UWorld* World, UParticleSystem* Particle, const FVector& Location);
	static void PlaySoundLoc(const UObject* WorldObject, USoundBase* SoundBase, const FVector& Location);

	UFUNCTION(BlueprintCallable)
	static UPaperSprite* LoadPaperSprite_FromFile(const FString& FullFilePath);

	UFUNCTION(BlueprintCallable)
	static int32 GetItemCount(const UWorld* World, const FName& ItemName);
};
