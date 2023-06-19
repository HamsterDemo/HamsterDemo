// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"


AMovableObject::AMovableObject()
{

}

void AMovableObject::BeginPlay()
{
	Super::BeginPlay();

}

void AMovableObject::IsInteractable()
{
	Super::IsInteractable();

	return true;
}

void AMovableObject::Interact()
{


}

void AMovableObject::EndInteract()
{

}