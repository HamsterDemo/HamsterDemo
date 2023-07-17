// Copyright Epic Games, Inc. All Rights Reserved.

#include "HamsterDemoCharacter.h"
#include "HamsterDemoProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectBaseUtility.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "DrawDebugHelpers.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AHamsterDemoCharacter

AHamsterDemoCharacter::AHamsterDemoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	static ConstructorHelpers::FClassFinder<UUserWidget> BP_UI_InteractablePopup(TEXT("/Game/UI/UI_InteractablePopup.UI_InteractablePopup_C")); 
	if (BP_UI_InteractablePopup.Succeeded())
	{
		InteractableTextClass = BP_UI_InteractablePopup.Class;
		UE_LOG(LogTemp, Log, TEXT("Widget Class succeeded"));
	}
	

}

void AHamsterDemoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	Mesh1P->SetHiddenInGame(false, true);

	if (IsValid(InteractableTextClass))
	{
		InteractableText = Cast<UUserWidget>(CreateWidget(GetWorld(), InteractableTextClass));

		if (IsValid(InteractableText))
		{
			UE_LOG(LogTemp, Log, TEXT("Widget valid"));
		}

		UE_LOG(LogTemp, Log, TEXT("Widget Class valid"));
	}
	
	characterPhysicsHandle = this->FindComponentByClass<UPhysicsHandleComponent>();
	if (characterPhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Physics Handle Component found by Class"));
	}

	characterGrabLocation = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("GrabLocation")));
	if (characterGrabLocation != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Grab Location Component found by Class"));
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void AHamsterDemoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHamsterDemoCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHamsterDemoCharacter::OnFire);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AHamsterDemoCharacter::OnSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AHamsterDemoCharacter::OffSprint);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AHamsterDemoCharacter::OnInteract);
	PlayerInputComponent->BindAction("EndInteraction", IE_Pressed, this, &AHamsterDemoCharacter::OnEndInteract);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AHamsterDemoCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AHamsterDemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHamsterDemoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHamsterDemoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHamsterDemoCharacter::LookUpAtRate);
}

void AHamsterDemoCharacter::Jump()
{
	if (IsInteractOn == false)
	{
		Super::Jump();
	}


}

void AHamsterDemoCharacter::OnSprint()
{
	UE_LOG(LogTemp, Log, TEXT("shift pressed"));
	GetCharacterMovement()->MaxWalkSpeed *= 1.5f;
}

void AHamsterDemoCharacter::OffSprint()
{
	UE_LOG(LogTemp, Log, TEXT("shift released"));
	GetCharacterMovement()->MaxWalkSpeed /= 1.5f;
}


void AHamsterDemoCharacter::OnInteract()
{
	if (isSuccessInteract)
	{
		UE_LOG(LogTemp, Log, TEXT("E pressed"));

		if (InteractableObj != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("interactable obj not null"));
			
			auto Movable = Cast<AMovableObject>(InteractableObj);
			if (Movable != nullptr) //interactable 중 movable 인지 확인
			{
				Movable->SetHandle(characterPhysicsHandle);
				Movable->SetHandleLocation(characterGrabLocation);
			}

			auto Talkable = Cast<ATalkableObject>(InteractableObj);
			if (Talkable != nullptr)
			{
				IsInteractOn = true;
			}
			
			
			InteractableObj->Interact();
		}
		
	}
}

void AHamsterDemoCharacter::OnEndInteract()
{
	if (InteractableObj != nullptr)
	{
		IsInteractOn = false;
		InteractableObj->EndInteract();
		InteractableObj = nullptr;
	}
	

}

// 총알 생성 로직
void AHamsterDemoCharacter::OnFire()
{
	if (IsInteractOn == false)
	{
		// try and fire a projectile
		if (ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AHamsterDemoProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}


		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

// 화면을 터치했을 때 불리는 콜백
void AHamsterDemoCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AHamsterDemoCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AHamsterDemoCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AHamsterDemoCharacter::MoveForward(float Value)
{
	if (IsInteractOn == false)
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorForwardVector(), Value);
		}
	}
}

void AHamsterDemoCharacter::MoveRight(float Value)
{
	if (IsInteractOn == false)
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorRightVector(), Value);
		}
	}
}

// 업데이트 문
void AHamsterDemoCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult hitResult;
	TryInteraction(hitResult);
}

AInteractableObject* AHamsterDemoCharacter::TraceInteractableObject(struct FHitResult& inHit)
{
	auto actor = inHit.GetActor();
	if (actor == nullptr)
		return nullptr;

	return Cast<AInteractableObject>(actor);
}

// 상호작용 종료 시 마무리 작업 친구들
void AHamsterDemoCharacter::ClearInteraction()
{ 
	
	if (InteractableText) //&& InteractableText->IsVisible()
	{
		InteractableText->RemoveFromParent();
	}

}

// 상호작용 가능 판정 성공 시 행동 
void AHamsterDemoCharacter::SetInteraction()
{
	const APlayerController* const PlayerController = Cast<const APlayerController>(GetController());
	FVector WorldLocation = InteractableObj->GetActorLocation();

	PlayerController->ProjectWorldLocationToScreen(WorldLocation, textLocation);

	if (InteractableText == nullptr)
		InteractableText = Cast<UUserWidget>(CreateWidget(GetWorld(), InteractableTextClass));

	InteractableText->SetPositionInViewport(textLocation);
	if (!InteractableText->IsVisible()) // 뷰 포트에 없으면 상호작용 가능 위젯 띄우기
	{
		InteractableText->AddToViewport(); // 위젯 띄우기
	}
}

// 상호작용 시도, 판정 실패 시 정보 클리어됨
void AHamsterDemoCharacter::TryInteraction(FHitResult hitResult)
{
	if (TraceOn(hitResult))
	{
		InteractableObj = TraceInteractableObject(hitResult); // 감지한 물체가 상호 작용 가능한 물체인 지 체크
		if (InteractableObj != nullptr)
		{
			isSuccessInteract = InteractableObj->IsInteractable(); // 상호작용 가능 판정
			if (isSuccessInteract)
			{
				SetInteraction();
				return;
			}
		}
	}

	ClearInteraction();

}

bool AHamsterDemoCharacter::TraceOn(struct FHitResult& OutHit)
{
	auto startPos = GetSpawnFVector();
	auto forwardVector = GetGunRightFVector();
	float TraceOffset = 500.0f;

	FVector endPos = ((forwardVector * TraceOffset) + startPos);

	FCollisionQueryParams CollisionParams;
	return GetWorld()->LineTraceSingleByChannel(OutHit, startPos, endPos, ECollisionChannel::ECC_WorldStatic, CollisionParams);
}

FVector AHamsterDemoCharacter::GetSpawnFVector()
{
	const FRotator SpawnRotation = GetControlRotation();

	FVector spawnPos;

	if (FP_MuzzleLocation != nullptr)
	{
		spawnPos = FP_MuzzleLocation->GetComponentLocation();
	}
	else
	{
		spawnPos = GetActorLocation();
	}

	return spawnPos + SpawnRotation.RotateVector(GunOffset);
}

// Gun's Y Vector
FVector AHamsterDemoCharacter::GetGunRightFVector()
{
	FVector RightVector; // 총이 바라보는 방향이어야 합니다.

	if (FP_MuzzleLocation != nullptr)
	{
		RightVector = FP_MuzzleLocation->GetRightVector(); 
	}
	else if (FP_Gun != nullptr)
	{
		RightVector = FP_Gun->GetRightVector();
	}
	else
	{
		RightVector = GetActorRightVector();
	}

	return RightVector;
}

void AHamsterDemoCharacter::TurnAtRate(float Rate)
{
	if (IsInteractOn == false)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AHamsterDemoCharacter::LookUpAtRate(float Rate)
{
	if (IsInteractOn == false)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

bool AHamsterDemoCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AHamsterDemoCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AHamsterDemoCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AHamsterDemoCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}
