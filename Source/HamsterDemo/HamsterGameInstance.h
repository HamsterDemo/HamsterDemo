// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "HamsterGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FHamsterDialogueData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHamsterDialogueData() : Name("StartName"), Line("StartLine") {} //초기 설정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Line;


};

/**
 *
 */
UCLASS()
class HAMSTERDEMO_API UHamsterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHamsterGameInstance();

	FHamsterDialogueData* GetDialogueData(int index);


private:
	UPROPERTY()
		class UDataTable* HamsterDialogueTable;

	TArray<FHamsterDialogueData*> DialogueArray;
	
};
