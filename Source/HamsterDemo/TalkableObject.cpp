// Fill out your copyright notice in the Description page of Project Settings.


#include "TalkableObject.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"


ATalkableObject::ATalkableObject()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_UI_TalkPopup(TEXT("/Game/UI/UI_TalkPopup.UI_TalkPopup_C"));

	if (BP_UI_TalkPopup.Succeeded())
	{
		TalkPopupClass = BP_UI_TalkPopup.Class;
		UE_LOG(LogTemp, Log, TEXT("Talk Class succeeded"));
	}
}

void ATalkableObject::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(TalkPopupClass))
	{
		TalkPopup = Cast<UUserWidget>(CreateWidget(GetWorld(), TalkPopupClass));

		if (IsValid(TalkPopup))
		{
			UE_LOG(LogTemp, Log, TEXT("Talk Popup valid"));
		}

		UE_LOG(LogTemp, Log, TEXT("Talk Class valid"));
	}
}


bool ATalkableObject::IsInteractable()
{
	Super::IsInteractable();

	return true;
}

void ATalkableObject::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("talkable interact"));

	if (!TalkPopup->IsVisible()) //ºäÆ÷Æ®¿¡ ¾øÀ¸¸é À§Á¬ ¶ç¿ì±â
	{
		TalkPopup->AddToViewport();
	}
}


void ATalkableObject::EndInteract()
{
	

	if (TalkPopup->IsVisible())
	{
		UE_LOG(LogTemp, Log, TEXT("talkable end interact - close talkpopup"));
		TalkPopup->RemoveFromParent();
	}

}