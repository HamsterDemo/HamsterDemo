// Copyright Epic Games, Inc. All Rights Reserved.

#include "HamsterDemoGameMode.h"
#include "HamsterDemoHUD.h"
#include "HamsterDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHamsterDemoGameMode::AHamsterDemoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AHamsterDemoHUD::StaticClass();
}
