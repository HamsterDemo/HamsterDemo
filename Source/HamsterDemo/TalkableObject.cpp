// Fill out your copyright notice in the Description page of Project Settings.


#include "TalkableObject.h"

ATalkableObject::ATalkableObject()
{
	//static ConstructorHelpers::FClassFinder<UUserWidget> BP_UI_TalkablePopup(TEXT(""));

}

void ATalkableObject::BeginPlay()
{
	Super::BeginPlay();

}

bool ATalkableObject::IsInteractable()
{
	Super::IsInteractable();

	return true;
}

void ATalkableObject::Interact()
{

}