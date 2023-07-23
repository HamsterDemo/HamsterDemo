// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObject.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

UPrimitiveComponent* AInteractableObject::GetComponent()
{
	auto result = this->GetComponentByClass(UPrimitiveComponent::StaticClass());
	if (result == nullptr)
	{
		return nullptr;
	}

	return Cast<UPrimitiveComponent>(result);
	
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AInteractableObject::IsInteractable()
{
	return IsInteracting == false;
}

bool AInteractableObject::CanMove()
{
	return true;
}

void AInteractableObject::Interact()
{
	IsInteracting = true;
}

void AInteractableObject::EndInteract()
{
	IsInteracting = false;
}

