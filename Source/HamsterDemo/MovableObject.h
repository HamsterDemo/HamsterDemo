// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "MovableObject.generated.h"

/**
 * 
 */
UCLASS()
class HAMSTERDEMO_API AMovableObject : public AInteractableObject
{
	GENERATED_BODY()
	

public:
	AMovableObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	bool IsInteractable();
	void SetHandle(UPhysicsHandleComponent* PhysicsHandle);
	void Interact() override;
	void EndInteract() override;

	UPhysicsHandleComponent* MovableHandle;
	UPrimitiveComponent* ComponentToGrab;

	
	
	



};
