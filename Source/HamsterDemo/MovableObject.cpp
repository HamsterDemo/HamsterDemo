// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"

#include "HamsterInteractorComponent.h"
#include "HamsterGrabbingPointComponent.h"


AMovableObject::AMovableObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
}

void AMovableObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (InteractorPhysicsHandleComponent == nullptr)
		return;

	if (InteractorPhysicsHandleComponent->GrabbedComponent)
	{
		AActor* Interactor = InteractorPhysicsHandleComponent->GetOwner();
		UHamsterGrabbingPointComponent* GrabbingPoint = Cast<UHamsterGrabbingPointComponent>(Interactor->GetComponentByClass(UHamsterGrabbingPointComponent::StaticClass()));
		FVector TargetLocation = GrabbingPoint->K2_GetComponentLocation();
		InteractorPhysicsHandleComponent->SetTargetLocation(TargetLocation);
	}
}

bool AMovableObject::IsInteractable(UHamsterInteractorComponent* InteractorComponent) const
{
	if (TargetComponent == nullptr)
		return false;

	AActor* Interactor = InteractorComponent->GetOwner();

	// there is already someone interacting with this
	if (InteractorPhysicsHandleComponent != nullptr)
		return false;

	if (Interactor->GetComponentByClass(UPhysicsHandleComponent::StaticClass()) == nullptr)
		return false;

	if (Interactor->GetComponentByClass(UHamsterGrabbingPointComponent::StaticClass()) == nullptr)
		return false;

	return true;
}

bool AMovableObject::BeginInteract(UHamsterInteractorComponent* InteractorComponent)
{
	AActor* Interactor = InteractorComponent->GetOwner();

	if (IsInteracting(InteractorComponent))
	{
		if (InteractorPhysicsHandleComponent != nullptr)
		{
			InteractorPhysicsHandleComponent->ReleaseComponent();
			InteractorPhysicsHandleComponent = nullptr;
		}

		if (TargetComponent != nullptr)
			TargetComponent->SetSimulatePhysics(false);
	}
	else
	{
		if (TargetComponent == nullptr)
			return false;

		InteractorPhysicsHandleComponent = Cast<UPhysicsHandleComponent>(Interactor->GetComponentByClass(UPhysicsHandleComponent::StaticClass()));
		if (InteractorPhysicsHandleComponent == nullptr)
			return false;

		TargetComponent->SetSimulatePhysics(true);
		InteractorPhysicsHandleComponent->GrabComponentAtLocationWithRotation(TargetComponent, NAME_None, GetActorLocation(), GetActorRotation());
	}

	
	return true;
}

void AMovableObject::EndInteract(UHamsterInteractorComponent* InteractorComponent)
{
	
}

bool AMovableObject::IsInteracting(UHamsterInteractorComponent* InteractorComponent) const
{
	return InteractorPhysicsHandleComponent != nullptr;
}