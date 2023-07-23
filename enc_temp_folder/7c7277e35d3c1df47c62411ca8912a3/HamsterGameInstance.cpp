// Fill out your copyright notice in the Description page of Project Settings.


#include "HamsterGameInstance.h"

UHamsterGameInstance::UHamsterGameInstance()
{
	FString DialogueDataPath = TEXT("/Game/FirstPerson/Data/HamsterDialogueSample.HamsterDialogueSample");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_HamDialogue(*DialogueDataPath);
	if (DT_HamDialogue.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Dialogue TableData succeeded"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Dialogue TableData not succeeded"));
	}

	HamsterDialogueTable = DT_HamDialogue.Object; //이건 뭘까 아직 모르겠음


}

//FHamsterDialogueData* GetDialogueData(FString Name)
//{
//	//return HamsterDialogueTable->FindRow<FHamsterDialogueData>(Name, TEXT(""));
//}