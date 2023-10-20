#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Portfolio01/Consts.h"
#include "MainCharacter.generated.h"

class USpotLightComponent;
class UInventory;
class UBoxComponent;
class UBuildSystem;
class USphereComponent;
class UCharacterAttribute;
enum class ECameraState : uint8;
class UCrossHairWidget;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UInstancedStaticMeshComponent;

UCLASS()
class PORTFOLIO01_API AMainCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const FHitInformation* HitInformation) override;

	FTransform GetCenterTransform();
	void ChangeAmbientSound(int32 Time);	// Time == 0 : Day, Time == 1 : Night

	UFUNCTION(BlueprintCallable)
	bool AddItemFromForge(FName ItemName, int32 Count);

private:	
	// Components
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;

	// Inputs
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* BuildAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SweepAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ForgeFirstAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ForgeSecondAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* FlashLightAction;

	bool bForgeFirstInput = false;
	bool bForgeSecondInput = false;
	bool bCanMove = true;

	void CheckForgeFirstInput(const FInputActionValue& Value);
	void CheckForgeSecondInput(const FInputActionValue& Value);

	// Interact
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* InteractSphere;

	UFUNCTION()
	void OnInteractSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Weapon
	UPROPERTY(VisibleAnywhere)
	AActor* OverlappedItem;

	// Attack
	float AimKeepingTimer = 0.f;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	FTimerHandle UpperBodyHandle;

	void TurnOffUpperBody();
	void CheckKeepingAimPose();

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* AttackSound;

	// Character Status
	UPROPERTY(EditDefaultsOnly)
	UCharacterAttribute* CharacterAttribute;
	
	// Open Inventory
	bool IsInvenOpen = false;
	void OpenInventory();

	// Play Ambient Sound
	UPROPERTY(EditDefaultsOnly)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DaySound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* NightSound;

	// Lights
	bool bHaveFlashLight;
	
	UPROPERTY(EditDefaultsOnly)
	USpotLightComponent* SpotLight;

	UPROPERTY(EditDefaultsOnly)
	float LightIntensyMax = 15000.f;

	void SetActiveFlashlight();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECharacterState CharacterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ECameraState CameraState;

	// Movements
	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = Movement)
	float AimWalkSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float IdleWalkSpeed = 500.f;

	UFUNCTION(BlueprintCallable)
	void SetOverlappedWeapon(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void SetDisOverlappedWeapon();

	void EquipWeapon();

	UPROPERTY(VisibleAnywhere)
	class ABaseItem* CurrentWeapon;

	// Aim
	void SetAimMode(const FInputActionValue& Value);
	void ChangeSpringArmOffset();

	FTimerHandle AimingTimerHandle;

	UPROPERTY(EditAnywhere)
	float AimingCameraDistance = 100.f;

	UPROPERTY(EditAnywhere)
	float IdleCameraDistance = 300.f;

	UPROPERTY(EditDefaultsOnly)
	float AimingDelta = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUpperBody = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CrossHairWidget;

	UPROPERTY()
	UCrossHairWidget* CrossHairInstance;

	// Attack
	FTimerHandle FireTimerHandle;
	FTimerHandle AimStartTimer;

	// 공격 인풋이 없을 때 조준 모션이 유지되는 시간
	UPROPERTY(EditDefaultsOnly)
	float AimKeepingTime = 5.f;

	void AttackStart(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Fire(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void AimStart();

	//Building
	UPROPERTY(EditAnywhere)
	UBuildSystem* BuildSystem;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BuildCenterBox;
	
	void SetBuildMode(const FInputActionValue& Value);

	// Collecting Items
	UPROPERTY(EditDefaultsOnly)
	UInstancedStaticMeshComponent* MapFoliage;
	
	void SearchFoliage();

	// Sweep
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* SweepMontage;
	
	void Sweep();
	
	UFUNCTION(BlueprintCallable)
	void CollisionEnable();
	
	UFUNCTION(BlueprintCallable)
	void CollisionDisable();

	UFUNCTION(BlueprintCallable)
	void MoveAvailable();

	// Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventory* InventoryAttr;

public:
	FORCEINLINE UCameraComponent* GetCameraComponent() { return CameraComponent; }
	UInventory* GetInventory();
};
