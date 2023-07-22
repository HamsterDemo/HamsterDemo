// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/SceneComponent.h"
#include "HamsterInteractableInterface.h"
#include "MovableObject.generated.h"

/**
 * 
 */
UCLASS()
class HAMSTERDEMO_API AMovableObject : public AActor, public IHamsterInteractableInterface
{
	GENERATED_BODY()
	

public:
	AMovableObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	void Interact() ;

protected:
	UPROPERTY(Transient)
	UPhysicsHandleComponent* InteractorPhysicsHandleComponent;

	UPROPERTY(EditDefaultsOnly)
	FVector GrabbedLocation;

	UPROPERTY(BlueprintReadWrite)
	UPrimitiveComponent* TargetComponent;

	// ///////////////////////////////////////////////
	// Inherited via IHamsterInteractableInterface
	virtual bool IsInteractable(class UHamsterInteractorComponent* InteractorComponent) const override;

	virtual bool BeginInteract(class UHamsterInteractorComponent* InteractorComponent) override;

	virtual void EndInteract(class UHamsterInteractorComponent* InteractorComponent) override;

	virtual bool IsInteracting(class UHamsterInteractorComponent* InteractorComponent) const override;
	// ///////////////////////////////////////////////
	// ///////////////////////////////////////////////

};
