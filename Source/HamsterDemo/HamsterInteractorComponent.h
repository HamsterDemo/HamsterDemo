// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HamsterInteractorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAMSTERDEMO_API UHamsterInteractorComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UHamsterInteractorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

private:
	bool DoLineTrace(FHitResult& OutHitResult);

	void OnBeginInteract();

	void OnEndInteract();

protected:
	UPROPERTY(EditAnywhere, Category = "Hamster|Interation")
	float InteractionTraceLength = 200.0f;

	UPROPERTY(EditAnywhere, Category="Hamster|Interation")
	TSubclassOf<class UUserWidget> InteractableFoundWidgetClass;

	UPROPERTY(Transient)
	class UUserWidget* InteractableFoundWidget;

	UPROPERTY(Transient)
	TScriptInterface<class IHamsterInteractableInterface> CurrentInteractableObject = nullptr;
};
