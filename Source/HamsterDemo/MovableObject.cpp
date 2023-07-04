// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"


AMovableObject::AMovableObject()
{

}

void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
	
	

}

bool AMovableObject::IsInteractable()
{
	Super::IsInteractable();

	return true;
}

void AMovableObject::Interact()
{
	//this->

}

void AMovableObject::EndInteract()
{

}