// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "GameplayWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HamsterInteractableInterface.h"
#include "TalkableObject.generated.h"

/**
 * 
 */
UCLASS()
class HAMSTERDEMO_API ATalkableObject : public AActor, public IHamsterInteractableInterface
{
	GENERATED_BODY()
	

public:
	ATalkableObject();

private:
	TArray<FString> dialogDatas;
	int currentDialogIndex = 0;


protected:
	virtual void BeginPlay() override;
	

protected:
	TSubclassOf<UUserWidget> TalkPopupClass;
	UPROPERTY(EditAnywhere)
		UUserWidget* TalkPopup;

protected:
	// ///////////////////////////////////////////////
	// Inherited via IHamsterInteractableInterface
	virtual bool IsInteractable(class UHamsterInteractorComponent* InteractorComponent) const override;

	virtual bool BeginInteract(class UHamsterInteractorComponent* InteractorComponent) override;

	virtual void EndInteract(class UHamsterInteractorComponent* InteractorComponent) override;

	virtual bool IsInteracting(class UHamsterInteractorComponent* InteractorComponent) const override;
	// ///////////////////////////////////////////////
	// ///////////////////////////////////////////////

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* TextLine;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* TextName;


};
