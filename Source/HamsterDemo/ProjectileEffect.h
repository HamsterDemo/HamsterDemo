// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "ProjectileEffect.generated.h"

UCLASS()
class HAMSTERDEMO_API AProjectileEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileEffect();

public:
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* Effect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystem* DamagedEffect;
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystem* UndamagedEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Create();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
