// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
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
	bool IsInteractable();
	void Interact() override;
	void EndInteract() override;

	
	
	



};
