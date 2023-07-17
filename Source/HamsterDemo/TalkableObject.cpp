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

	//�̾߱� �������� true, ������ false
	//�̾߱� �������� Interact() �ٽ� �ؼ� �̾߱� ����
	//�̾߱� �������� false�ؼ� EndInteract()�� ����
}

void ATalkableObject::Interact()
{
	Super::Interact();

	if (!TalkPopup->IsVisible()) //����Ʈ�� ������ ���� ����
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