// Fill out your copyright notice in the Description page of Project Settings.


#include "HamsterInteractorComponent.h"

// UE4
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/InputComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "Blueprint/UserWidget.h"

// PS
#include "HamsterInteractableInterface.h"

// Sets default values for this component's properties
UHamsterInteractorComponent::UHamsterInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHamsterInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UHamsterInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentInteractableObject != nullptr)
	{
		// if still is interacting, return
		if (CurrentInteractableObject->IsInteracting(this) == true)
		{
			// 어차피 되지도 않음. ㅋㅋ
			// 
			//if (InteractableFoundWidget != nullptr)
			//{
			//	APawn* OwnerPawn = Cast<APawn>(GetOwner());
			//	if (OwnerPawn == nullptr)
			//		return;

			//	const APlayerController* PlayerController = Cast<const APlayerController>(OwnerPawn->GetController());
			//	if (PlayerController == nullptr)
			//		return;

			//	AActor* InteractableActor = Cast<AActor>(CurrentInteractableObject.GetObject());
			//	FVector InteractableWorldLocation = InteractableActor->GetActorLocation();

			//	FVector2D DesiredWidgetLocation;
			//	PlayerController->ProjectWorldLocationToScreen(InteractableWorldLocation, OUT DesiredWidgetLocation);

			//	InteractableFoundWidget->SetPositionInViewport(DesiredWidgetLocation);
			//}

			return;
		}

		CurrentInteractableObject = nullptr;
		if (InteractableFoundWidget != nullptr)
		{
			InteractableFoundWidget->RemoveFromViewport();
			InteractableFoundWidget = nullptr;
		}
	}

	FHitResult HitResult;
	if (DoLineTrace(OUT HitResult) == false)
		return;

	IHamsterInteractableInterface* InteractableObject = Cast<IHamsterInteractableInterface>(HitResult.Actor);
	if (InteractableObject == nullptr)
		return;

	if (InteractableObject->IsInteractable(this) == false)
		return;

	CurrentInteractableObject.SetInterface(InteractableObject);
	CurrentInteractableObject.SetObject(HitResult.Actor.Get());

	if (InteractableFoundWidget == nullptr)
	{
		InteractableFoundWidget = CreateWidget<UUserWidget>(GetWorld(), InteractableFoundWidgetClass);
		InteractableFoundWidget->AddToViewport();
	}
	
}

void UHamsterInteractorComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &UHamsterInteractorComponent::OnBeginInteract);
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &UHamsterInteractorComponent::OnEndInteract);
}

void UHamsterInteractorComponent::OnBeginInteract()
{
	if (CurrentInteractableObject == nullptr)
		return;

	CurrentInteractableObject->BeginInteract(this);
}

void UHamsterInteractorComponent::OnEndInteract()
{
	if (CurrentInteractableObject == nullptr)
		return;

	CurrentInteractableObject->EndInteract(this);
}


bool UHamsterInteractorComponent::DoLineTrace(FHitResult& OutHitResult)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return false;

	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (PlayerController == nullptr)
		return false;

	APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;
	if (PlayerCameraManager == nullptr)
		return false;

	FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	FVector CameraForward = PlayerCameraManager->GetCameraRotation().Vector();

#if !UE_BUILD_SHIPPING && !WITH_EDITOR
	UKismetSystemLibrary::DrawDebugLine
	(
		this,
		CameraLocation,
		CameraLocation + CameraForward * InteractionTraceLength,
		FLinearColor::Red,
		1.0f,
		1.0f
	);
#endif

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(OUT OutHitResult, CameraLocation, CameraLocation + CameraForward * InteractionTraceLength, ECollisionChannel::ECC_WorldStatic, QueryParams);

	return OutHitResult.Actor.IsValid();
}

