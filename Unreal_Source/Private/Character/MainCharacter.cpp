#include "Character/MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Items/BaseItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Character/EnemyBase.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttComponents/BuildSystem.h"
#include "Portfolio01/Consts.h"
#include "Utils/CommonUtils.h"
#include "Widget/CrossHairWidget.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "AttComponents/CharacterAttribute.h"
#include "AttComponents/Inventory.h"
#include "Buildings/Foliage.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameInstance/MainGameInstance.h"
#include "Utils/DebugUtils.h"
#include "Utils/UIUtils.h"
#include "Widget/MainHud.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Widget/BloodScreen.h"
#include "Widget/ForgeWidget.h"
#include "Widget/InventoryWidget.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	CharacterAttribute = CreateDefaultSubobject<UCharacterAttribute>(TEXT("Character Attribute"));

	BuildSystem = CreateDefaultSubobject<UBuildSystem>(TEXT("Build System"));

	BuildCenterBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Build Center Box"));
	BuildCenterBox->SetupAttachment(GetRootComponent());
	BuildCenterBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Sphere"));
	InteractSphere->SetupAttachment(GetRootComponent());
	InteractSphere->SetGenerateOverlapEvents(true);

	InventoryAttr = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AUDIO COMPONENT"));
	AudioComponent->SetupAttachment(GetRootComponent());

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot Light"));
	SpotLight->SetupAttachment(GetRootComponent());
}

void AMainCharacter::CheckForgeFirstInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		bForgeFirstInput = true;
	}
}

void AMainCharacter::CheckForgeSecondInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		bForgeSecondInput = true;
	}
	else
	{
		bForgeSecondInput = false;
	}

	if (bForgeFirstInput && bForgeSecondInput)
	{
		UUIUtils::SetActive(UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->Forge, true);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

		UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->ForgeWidget->RefreshListElement();
		bCanMove = false;

		UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->ForgeWidget->OnCloseFunc.BindLambda([this]()
		{
			bCanMove = true;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
		});

		bForgeFirstInput = false;
		bForgeSecondInput = false;
	}
}

void AMainCharacter::OnInteractSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	IInteract* Interact = Cast<IInteract>(OtherActor);
	if (!Interact) return;

	Interact->GetInteractive();
}

void AMainCharacter::OnInteractSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteract* Interact = Cast<IInteract>(OtherActor);
	if (!Interact) return;
	Interact->GetUnInteractive();
}

void AMainCharacter::TurnOffUpperBody()
{
	bUpperBody = false;
}

void AMainCharacter::CheckKeepingAimPose()
{
	if (AimKeepingTimer < AimKeepingTime)
	{
		AimKeepingTimer += GetWorld()->DeltaTimeSeconds;
		return;
	}

	GetWorldTimerManager().ClearTimer(AimStartTimer);
	UCommonUtils::PlayMontage(GetMesh()->GetAnimInstance(), AttackMontage, FSectionName::AimToIdle);
	CharacterState = ECharacterState::WeaponIdle;
}

void AMainCharacter::OpenInventory()
{
	IsInvenOpen = !IsInvenOpen;
	if (IsInvenOpen)
	{
		UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->InventoryWidget->RefreshListBox();
	}
	else
	{
		UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->InventoryWidget->RemoveAllItems();
	}
	UUIUtils::SetActive(UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->Inventory, IsInvenOpen);
}

void AMainCharacter::SetActiveFlashlight()
{
	if (bForgeFirstInput) return;
	
	if (!bHaveFlashLight)
	{
		if (InventoryAttr->HaveItem(FName("Flashlight")))
		{
			bHaveFlashLight = true;
		}
		else
		{
			SpotLight->Intensity = 0;
			PRINT_SCR("Don't have flashlight!");
			return;
		}
	}

	if (SpotLight->Intensity == 0)
	{
		SpotLight->SetIntensity(LightIntensyMax);
	}
	else if (SpotLight->Intensity == LightIntensyMax)
	{
		SpotLight->SetIntensity(0);
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			Subsystem && CharacterMappingContext)
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}

	CharacterState = ECharacterState::Idle;
	CameraState = ECameraState::Idle;

	Tags.Add(FTag::Player);

	if (InteractSphere)
	{
		InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnInteractSphereBeginOverlap);
		InteractSphere->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnInteractSphereEndOverlap);
	}

	AudioComponent->SetSound(DaySound);
	AudioComponent->Play();
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	if (!bCanMove) return;

	FVector2D CurValue = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator ControlRotation = GetControlRotation();

		const FRotator RightRotation(0, ControlRotation.Yaw, ControlRotation.Roll);
		const FVector RightVector = UKismetMathLibrary::GetRightVector(RightRotation);
		AddMovementInput(RightVector, CurValue.X);

		const FRotator ForwardRotation(0, ControlRotation.Yaw, 0);
		const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ForwardRotation);
		AddMovementInput(ForwardVector, CurValue.Y);
	}
}

void AMainCharacter::Turn(const FInputActionValue& Value)
{
	if (!bCanMove) return;

	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X * -1);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void AMainCharacter::SetOverlappedWeapon(AActor* Actor)
{
	OverlappedItem = Actor;
}

void AMainCharacter::SetDisOverlappedWeapon()
{
	OverlappedItem = nullptr;
}

void AMainCharacter::EquipWeapon()
{
	if (!OverlappedItem) return;
	if (ABaseItem* Item = Cast<ABaseItem>(OverlappedItem))
	{
		OverlappedItem = nullptr;

		switch (Item->GetItemType())
		{
		case EItemType::Weapon:
			{
				CurrentWeapon = Item;
				CharacterState = ECharacterState::WeaponIdle;
				CurrentWeapon->Equip(GetMesh(), FSocketName::RifleEquipSocket, this);

				// 조준점 UI 추가		
				UUIUtils::SetActive(UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->Crosshair, true);
				break;
			}
		case EItemType::Sweep:
			{
				CurrentWeapon = Item;
				CharacterState = ECharacterState::Sweep;
				CurrentWeapon->Equip(GetMesh(), FSocketName::SweepSocket, this);

				break;
			}
		case EItemType::ETC:
			{
				Item->Equip(nullptr, FName(), nullptr);
				break;
			}
		default:
			break;
		}
	}
}

void AMainCharacter::SetAimMode(const FInputActionValue& Value)
{
	if (CurrentWeapon == nullptr || CharacterState != ECharacterState::WeaponIdle || CharacterState !=
		ECharacterState::WeaponAim)
		return;

	GetWorldTimerManager().SetTimer(AimingTimerHandle, this, &AMainCharacter::ChangeSpringArmOffset,
	                                GetWorld()->DeltaTimeSeconds / 5.f, true);
}

void AMainCharacter::ChangeSpringArmOffset()
{
	if (CameraState == ECameraState::Zoom)
	{
		SpringArmComponent->TargetArmLength += AimingDelta;
		GetCharacterMovement()->MaxWalkSpeed = IdleWalkSpeed;
		Cast<UCrossHairWidget>(UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->Crosshair)->SetIdlePosition();
		if (SpringArmComponent->TargetArmLength == IdleCameraDistance)
		{
			GetWorldTimerManager().ClearTimer(AimingTimerHandle);
			CharacterState = ECharacterState::WeaponIdle;
			CameraState = ECameraState::Idle;
			bUseControllerRotationYaw = false;
		}
	}
	else if (CameraState == ECameraState::Idle)
	{
		SpringArmComponent->TargetArmLength -= AimingDelta;
		GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
		Cast<UCrossHairWidget>(UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->Crosshair)->SetAttackPosition();

		if (SpringArmComponent->TargetArmLength == AimingCameraDistance)
		{
			GetWorldTimerManager().ClearTimer(AimingTimerHandle);
			CharacterState = ECharacterState::WeaponAim;
			CameraState = ECameraState::Zoom;
			bUseControllerRotationYaw = true;
		}
	}
}

void AMainCharacter::AttackStart(const FInputActionValue& Value)
{
	if (!bCanMove) return;

	if (CharacterState == ECharacterState::WeaponAim)
	{
		Fire(nullptr, false);
	}
	else if (CurrentWeapon && CurrentWeapon->GetItemType() == EItemType::Weapon)
	{
		bUseControllerRotationYaw = false;
		CharacterState = ECharacterState::WeaponAim;
		UCommonUtils::PlayMontage(GetMesh()->GetAnimInstance(), AttackMontage, FSectionName::IdleToAim);
	}
	else if (CurrentWeapon && CurrentWeapon->GetItemType() == EItemType::Sweep)
	{
		Sweep();
	}
}

void AMainCharacter::Fire(UAnimMontage* Montage, bool bInterrupted)
{
	if (!CurrentWeapon) return;
	UWorld* World = GetWorld();
	if (!World) return;

	bUpperBody = true;
	UCommonUtils::PlayMontage(GetMesh()->GetAnimInstance(), AttackMontage, FSectionName::Fire);
	AimKeepingTimer = 0.f;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D CrosshairOffset = FVector2D(0, 20);
	FVector2D CrossHairLocation = (ViewportSize - CrosshairOffset) / 2.f;

	if (FVector CrossHairWorldPosition, CrossHairWorldDirection; UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrossHairLocation, CrossHairWorldPosition, CrossHairWorldDirection))
	{
		const USkeletalMeshSocket* MuzzleSocket = CurrentWeapon->GetMesh()->GetSocketByName(FSocketName::MuzzleSocket);
		const FTransform SocketTransform = MuzzleSocket->GetSocketTransform(CurrentWeapon->GetMesh());

		FHitResult ScreenTraceHit;
		const FVector Start = SocketTransform.GetLocation();
		const FVector End = Start + (CrossHairWorldDirection + FVector(0.05f, 0, 0)) * 50'000.f;
		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECC_Visibility);
		if (ScreenTraceHit.bBlockingHit)
		{
			if (IHitInterface* HitInterface = Cast<IHitInterface>(Cast<AEnemyBase>(ScreenTraceHit.GetActor())))
			{
				UGameplayStatics::ApplyDamage(ScreenTraceHit.GetActor(), 10.f, GetInstigator()->GetController(), this,
				                              UDamageType::StaticClass());
				FHitInformation* HitInformation = new FHitInformation();
				HitInformation->AttackedActor = this;

				HitInterface->GetHit(HitInformation);
			}

			UCommonUtils::PlayParticle(World, MuzzleFlash, Start);
			UCommonUtils::PlaySoundLoc(this, AttackSound, GetActorLocation());
		}
	}
}

void AMainCharacter::AimStart()
{
	AimKeepingTimer = 0;
	GetWorldTimerManager().SetTimer(AimStartTimer, this, &AMainCharacter::CheckKeepingAimPose,
	                                GetWorld()->DeltaTimeSeconds, true);
}

void AMainCharacter::SetBuildMode(const FInputActionValue& Value)
{
	BuildSystem->SetBuildMode(!BuildSystem->GetBuildMode());
}

void AMainCharacter::SearchFoliage()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector StartPosition = BuildCenterBox->GetComponentTransform().GetLocation();
	FVector ForwardVector = CameraComponent->GetComponentTransform().GetRotation().GetForwardVector() * 700.f;

	FHitResult HitResult;
	World->LineTraceSingleByChannel(HitResult, StartPosition, StartPosition + ForwardVector,
	                                ECollisionChannel::ECC_WorldStatic);
	if (HitResult.bBlockingHit)
	{
		MapFoliage = Cast<UInstancedStaticMeshComponent>(HitResult.Component);
		UDebugUtils::DebugLine(World, StartPosition, HitResult.Location);
		if (MapFoliage)
		{
			FString MeshName = MapFoliage->GetStaticMesh()->GetName();
			if (!UCommonUtils::GetCurGameInstance(World)->FoliageMesh.Contains(*MeshName)) return;

			FTransform InstanceTransform;
			MapFoliage->GetInstanceTransform(HitResult.Item, InstanceTransform);

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			if (AFoliage* CreatedFoliage = World->SpawnActor<AFoliage>(
				UCommonUtils::GetCurGameInstance(World)->FoliageClass, InstanceTransform, SpawnParameters))
			{
				UCommonUtils::GetCurGameInstance(World)->CreatedFoliage.Add(CreatedFoliage);
				CreatedFoliage->SetStaticMesh(UCommonUtils::GetCurGameInstance(World)->FoliageMesh[*MeshName]);
				CreatedFoliage->SetActorTransform(InstanceTransform);
			}

			MapFoliage->RemoveInstance(HitResult.Item);
		}
	}
}

void AMainCharacter::Sweep()
{
	bCanMove = false;
	UCommonUtils::PlayMontage(GetMesh()->GetAnimInstance(), SweepMontage);
}

void AMainCharacter::CollisionEnable()
{
	CurrentWeapon->SetWeaponCollision(true);
}

void AMainCharacter::CollisionDisable()
{
	CurrentWeapon->SetWeaponCollision(false);
}

void AMainCharacter::MoveAvailable()
{
	bCanMove = true;
}

UInventory* AMainCharacter::GetInventory()
{
	return InventoryAttr;
}

FTransform AMainCharacter::GetCenterTransform()
{
	if (!BuildCenterBox)
	{
		PRINT_SCR("!!! BuildCenterBox is null !!!");
		return FTransform();
	}

	return BuildCenterBox->GetComponentTransform();
}

void AMainCharacter::ChangeAmbientSound(int32 Time)
{
	switch (Time)
	{
	case 0:
		{
			AudioComponent->SetSound(DaySound);
			AudioComponent->Play();
			break;
		}
	case 1:
		{
			AudioComponent->SetSound(NightSound);
			AudioComponent->Play();
			break;
		}
	default:
		{
			break;
		}
	}
}

bool AMainCharacter::AddItemFromForge(FName ItemName, int32 Count)
{
	if (ItemName.IsNone() || Count < 0) return false;

	InventoryAttr->InputItem(ItemName, Count);
	
	return true;
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterState == ECharacterState::Sweep)
	{
		SearchFoliage();
	}

	SpotLight->SetWorldRotation(GetControlRotation());
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Turn);
		EInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
		EInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AMainCharacter::EquipWeapon);
		EInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AMainCharacter::SetAimMode);
		EInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMainCharacter::AttackStart);
		EInputComponent->BindAction(BuildAction, ETriggerEvent::Started, this, &AMainCharacter::SetBuildMode);
		EInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMainCharacter::OpenInventory);
		EInputComponent->BindAction(ForgeFirstAction, ETriggerEvent::Started, this,
		                            &AMainCharacter::CheckForgeFirstInput);
		EInputComponent->BindAction(ForgeSecondAction, ETriggerEvent::Started, this,
		                            &AMainCharacter::CheckForgeSecondInput);
		EInputComponent->BindAction(ForgeSecondAction, ETriggerEvent::Started, this,
		                            &AMainCharacter::SetActiveFlashlight);
	}
}

void AMainCharacter::GetHit(const FHitInformation* HitInformation)
{
	if (!HitInformation->AttackedActor->Tags.Contains(FTag::Enemy)) return;
	UCommonUtils::GetCurGameInstance(GetWorld())->MainHud->BloodScreenWidget->TurnOnBloodImage();
}
