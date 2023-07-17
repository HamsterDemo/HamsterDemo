// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"


AMovableObject::AMovableObject()
{

}

void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
}

void AMovableObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MovableHandle == nullptr)
		return;

	if (MovableHandle->GrabbedComponent)
	{
		// 잡고있는 쪽으로 오브젝트 이동
		if (HookingComponent != nullptr)
		{
			FVector TargetLocation = HookingComponent->GetComponentLocation();
			UE_LOG(LogTemp, Log, TEXT("%s"), *TargetLocation.ToString());

			MovableHandle->SetTargetLocation(TargetLocation);
			UE_LOG(LogTemp, Log, TEXT("MovableHandle Grabbed Component"));
		}
	}
}

bool AMovableObject::IsInteractable()
{
	return Super::IsInteractable();

}

void AMovableObject::SetHandle(UPhysicsHandleComponent* PhysicsHandle)
{ 
	MovableHandle = PhysicsHandle;
}

void AMovableObject::SetHandleLocation(USceneComponent* GrabLocation)
{
	HookingComponent = GrabLocation;
}

void AMovableObject::Interact()
{
	Super::Interact();

	if (MovableHandle == nullptr)
		return;

	auto targetComponent = this->GetComponent();
	if (targetComponent == nullptr)
		return;

	targetComponent->SetSimulatePhysics(true);
	MovableHandle->GrabComponentAtLocationWithRotation(targetComponent, NAME_None, GetActorLocation(), GetActorRotation());
	UE_LOG(LogTemp, Log, TEXT("Hook Complete"));
}

void AMovableObject::EndInteract()
{
	Super::EndInteract();

	auto targetComponent = this->GetComponent();
	if (targetComponent == nullptr)
		return;

	targetComponent->SetSimulatePhysics(false);

	MovableHandle->ReleaseComponent();
}