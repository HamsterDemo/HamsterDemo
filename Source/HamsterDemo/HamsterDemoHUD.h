// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HamsterDemoHUD.generated.h"

UCLASS()
class AHamsterDemoHUD : public AHUD
{
	GENERATED_BODY()

public:
	AHamsterDemoHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

