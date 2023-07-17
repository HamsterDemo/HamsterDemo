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

	TextName = Cast<UTextBlock>(TalkPopup->GetWidgetFromName(TEXT("Name")));
	TextLine = Cast<UTextBlock>(TalkPopup->GetWidgetFromName(TEXT("Line")));
	

	TextName->SetText(FText::FromString("test name"));
	TextLine->SetText(FText::FromString("test line"));
	
}


bool ATalkableObject::IsInteractable()
{
	
	
	return CanTalkMore == true;

	//이야기 남았으면 true, 끝나면 false
	//이야기 남았으면 Interact() 다시 해서 이야기 진행
	//이야기 끝났으면 false해서 EndInteract()로 가게
}

void ATalkableObject::Interact()
{
	Super::Interact();

	if (!TalkPopup->IsVisible()) //뷰포트에 없으면 위젯 띄우기
	{
		UE_LOG(LogTemp, Log, TEXT("talkable interact"));
		TalkPopup->AddToViewport();
		TextName->SetText(FText::FromString("test name2"));
		TextLine->SetText(FText::FromString("test line2"));
		CanTalkMore = true;

	}
	else
	{
		TextName->SetText(FText::FromString("test name3"));
		TextLine->SetText(FText::FromString("test line3"));
		CanTalkMore = false;
	}
	
}

bool ATalkableObject::CanMove()
{
	return false;
}


void ATalkableObject::EndInteract()
{
	Super::EndInteract();

	if (TalkPopup->IsVisible())
	{
		UE_LOG(LogTemp, Log, TEXT("talkable end interact - close talkpopup"));
		TalkPopup->RemoveFromParent();
	}
	CanTalkMore = true;

}