#include "Utils/CommonUtils.h"

#include "PaperSprite.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AttComponents/Inventory.h"
#include "Character/MainCharacter.h"
#include "GameInstance/MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

float UCommonUtils::PlayMontage(UAnimInstance* Instance, UAnimMontage* Montage, FName SectionName)
{
	if (!Instance || !Montage) return 0.f;

	if (SectionName.IsNone())
		SectionName = FName("Default");
	
	Instance->Montage_Play(Montage);
	Instance->Montage_JumpToSection(SectionName, Montage);
	
	const int32 Index = Montage->GetSectionIndex(SectionName);
	return Montage->GetSectionLength(Index);
}

UMainGameInstance* UCommonUtils::GetCurGameInstance(const UWorld* World)
{
	return Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(World));
}

void UCommonUtils::PlayParticle(const UWorld* World, UParticleSystem* Particle, const FVector& Location)
{
	if (!World || !Particle) return;
	UGameplayStatics::SpawnEmitterAtLocation(World, Particle, Location);
}

void UCommonUtils::PlaySoundLoc(const UObject* WorldObject, USoundBase* SoundBase, const FVector& Location)
{
	if (!WorldObject || !SoundBase) return;
	UGameplayStatics::PlaySoundAtLocation(WorldObject, SoundBase, Location);
}

UPaperSprite* UCommonUtils::LoadPaperSprite_FromFile(const FString& FullFilePath)
{
	TSoftObjectPtr<UPaperSprite> SoftPtr(FullFilePath);
	UPaperSprite* Sprite = SoftPtr.Get();
	if (!Sprite)
	{
		Sprite = SoftPtr.LoadSynchronous();
	}

	return Sprite;
}

int32 UCommonUtils::GetItemCount(const UWorld* World, const FName& ItemName)
{
	return GetCurGameInstance(World)->GetPlayer()->GetInventory()->GetItemCount(ItemName);
}
