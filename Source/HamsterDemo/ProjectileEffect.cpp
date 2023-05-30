// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect.h"

// Sets default values
AProjectileEffect::AProjectileEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	Effect->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DAMAGED(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ability/Stun/P_Stun_Stars_Base.P_Stun_Stars_Base"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_UNDAMAGED(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_Heal_Shrine_Start.P_Heal_Shrine_Start"));

	if (P_DAMAGED.Succeeded())
	{
		DamagedEffect = P_DAMAGED.Object;
	}
	if (P_UNDAMAGED.Succeeded())
	{
		UndamagedEffect = P_UNDAMAGED.Object;
	}

	Create();
}

// Called when the game starts or when spawned
void AProjectileEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileEffect::Create()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		const FRotator SpawnRotation = GetActorRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		World->SpawnActor<UParticleSystem>(DamagedEffect->GetClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);
	
		Effect->SetTemplate(DamagedEffect);
		Effect->Activate(true);
	}
}


