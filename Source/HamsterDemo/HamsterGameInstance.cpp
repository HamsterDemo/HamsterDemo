// Fill out your copyright notice in the Description page of Project Settings.


#include "HamsterGameInstance.h"

UHamsterGameInstance::UHamsterGameInstance()
{
	FString DialogueDataPath = TEXT("/Game/FirstPerson/Data/HamsterDialogueSample2.HamsterDialogueSample2");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_HamDialogue(*DialogueDataPath);
	if (DT_HamDialogue.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Dialogue TableData succeeded"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Dialogue TableData not succeeded"));
		return;
	}

	HamsterDialogueTable = DT_HamDialogue.Object;
	HamsterDialogueTable->GetAllRows("", OUT DialogueArray);
}

FHamsterDialogueData* UHamsterGameInstance::GetDialogueData(int index)
{
	if (DialogueArray.Num() <= index)
	{
		return HamsterDialogueTable->FindRow<FHamsterDialogueData>(FName(*(FString::FormatAsNumber(index))), TEXT(""));
	}
	else
	{
		return DialogueArray[index];
	}
} //항목 하나씩 가져오는 함수


TArray<FHamsterDialogueData*> UHamsterGameInstance::GetAllDialogueData()
{
	return DialogueArray;

} //DialogueArray 전체를 가져오는 함수

