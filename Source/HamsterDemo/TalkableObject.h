// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "TalkableObject.generated.h"

/**
 * 
 */
UCLASS()
class HAMSTERDEMO_API ATalkableObject : public AInteractableObject
{
	GENERATED_BODY()
	

public:
	ATalkableObject();


protected:
	virtual void BeginPlay() override;

	void Interact();


public:
	bool IsInteractable();

protected:
	TSubclassOf<UUserWidget> TalkPopupClass;
	UUserWidget* TalkPopup;

};
