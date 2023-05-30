// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAMSTERDEMO_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	

//protected:
//	UPROPERTY(meta = (BindWidget))
//		class UTextBlock* InteractableText;

	void NativeConstruct() override;
};
