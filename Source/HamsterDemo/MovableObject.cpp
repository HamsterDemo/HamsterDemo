// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"


AMovableObject::AMovableObject()
{

}

void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
	
	HookedComponent = this->GetComponent();
}

void AMovableObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MovableHandle == nullptr)
		return;

	if (MovableHandle->GrabbedComponent)
	{
		// ����ִ� ������ ������Ʈ �̵�
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
	return true;
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
	//PhysicsHandle�� �Ű������� �޾Ƽ�, interactableObj(this)�� getcomponent

	if (MovableHandle == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("MovableHandle nullptr"));
		return;
	}
	if (HookedComponent == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ComponentToGrab nullptr"));
		return;
	}

	auto hookedOwner = HookedComponent->GetOwner();
	if (hookedOwner == nullptr)
		return;

	MovableHandle->GrabComponentAtLocation(HookedComponent, NAME_None, hookedOwner->GetActorLocation());
	UE_LOG(LogTemp, Log, TEXT("Hook Complete"));
}

void AMovableObject::EndInteract()
{

}