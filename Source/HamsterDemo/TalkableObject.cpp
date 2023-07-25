// Fill out your copyright notice in the Description page of Project Settings.


#include "TalkableObject.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"


ATalkableObject::ATalkableObject()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_UI_TalkPopup(TEXT("/Game/UI/UI_TalkPopup.UI_TalkPopup_C"));

	if (BP_UI_TalkPopup.Succeeded())
	{
		TalkPopupClass = BP_UI_TalkPopup.Class;
		UE_LOG(LogTemp, Log, TEXT("Talk Class succeeded"));
	}


	CurrentDialogIndex = 0;

}

void ATalkableObject::BeginPlay()
{
	auto HamsterGameInstance = Cast<UHamsterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	DialogDatas = HamsterGameInstance->GetAllDialogueData();

	
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
}

bool ATalkableObject::IsInteractable(UHamsterInteractorComponent* InteractorComponent) const
{
	return CurrentDialogIndex < DialogDatas.Num();
}

bool ATalkableObject::BeginInteract(UHamsterInteractorComponent* InteractorComponent)
{
	if (!TalkPopup->IsVisible()) //ºäÆ÷Æ®¿¡ ¾øÀ¸¸é À§Á¬ ¶ç¿ì±â
	{
		TalkPopup->AddToViewport();
	}

	if (CurrentDialogIndex >= DialogDatas.Num())
	{
		CurrentDialogIndex = 0;
		
		if (TalkPopup->IsVisible())
		{
			UE_LOG(LogTemp, Log, TEXT("talkable end interact - close talkpopup"));
			TalkPopup->RemoveFromParent();
		}
	}
	else
	{
		TextName->SetText(FText::FromString(DialogDatas[CurrentDialogIndex]->Name));
		TextLine->SetText(FText::FromString(DialogDatas[CurrentDialogIndex++]->Line));
	}

	return true;
}

void ATalkableObject::EndInteract(UHamsterInteractorComponent* InteractorComponent)
{
	
}

bool ATalkableObject::IsInteracting(UHamsterInteractorComponent* InteractorComponent) const
{
	return CurrentDialogIndex > 0;
}
