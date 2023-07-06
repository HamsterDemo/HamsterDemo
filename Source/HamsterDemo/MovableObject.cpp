// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"


AMovableObject::AMovableObject()
{

}

void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
	
	ComponentToGrab = this->GetComponent();

}

void AMovableObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MovableHandle == nullptr)
	{
		return;
	}

	if (MovableHandle->GrabbedComponent)
	{
		// 잡고있는 쪽으로 오브젝트 이동
		//MovableHandle->SetTargetLocation();
		UE_LOG(LogTemp, Log, TEXT("MovableHandle Grabbed Component"));
	}

}

bool AMovableObject::IsInteractable()
{
	Super::IsInteractable();

	return true;
}

void AMovableObject::SetHandle(UPhysicsHandleComponent* PhysicsHandle)
{ 
	MovableHandle = PhysicsHandle;

}


void AMovableObject::Interact()
{
	//PhysicsHandle을 매개변수로 받아서, interactableObj(this)를 getcomponent

	if (MovableHandle == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("MovableHandle nullptr"));
		return;
	}
	if (ComponentToGrab == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ComponentToGrab nullptr"));
		return;
	}

	MovableHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
}

void AMovableObject::EndInteract()
{

}