// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AMonsterAIController::AMonsterAIController()
{
	RepeatInterval = 3.0f;
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMonsterAIController::OnRepeatTimer, RepeatInterval, true);


}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AMonsterAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	if (CurrentPawn == nullptr)
		return;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("NavSystem nullptr"));
		return;
	}

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		//UNavigationSystemV1::SimpleMoveToLocation(this, NextLocation.Location);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		UE_LOG(LogTemp, Log, TEXT("Next Location: %s"), *NextLocation.Location.ToString());
	}
}
