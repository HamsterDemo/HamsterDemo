// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HamsterInteractableInterface.generated.h"

UINTERFACE(Blueprintable)
class UHamsterInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

class IHamsterInteractableInterface
{
    GENERATED_BODY()

public:
    virtual bool IsInteractable(class UHamsterInteractorComponent* Interactor) const = 0;

    virtual bool BeginInteract(class UHamsterInteractorComponent* Interactor) = 0;

    virtual void EndInteract(class UHamsterInteractorComponent* Interactor) = 0;

    virtual bool IsInteracting(class UHamsterInteractorComponent* Interactor) const = 0;
};