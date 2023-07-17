// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

UCLASS()
class HAMSTERDEMO_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual UPrimitiveComponent* GetComponent();

	//UPrimitiveComponent* component = actor->GetComponentByClass(UprimitiveComponent*);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool IsInteractable();
	virtual bool CanMove();
	virtual void Interact();
	virtual void EndInteract();

private:
	bool IsInteracting = false;

};
