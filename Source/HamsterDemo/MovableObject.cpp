// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"


AMovableObject::AMovableObject()
{

}

void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
	
	MovableHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("MovableHandle")); //컴포넌트 추가해야함


}

bool AMovableObject::IsInteractable()
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