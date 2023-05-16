// Copyright Epic Games, Inc. All Rights Reserved.

#include "HamsterDemoProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"


AHamsterDemoProjectile::AHamsterDemoProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AHamsterDemoProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 1.0f;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	Effect->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DAMAGED(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ability/Stun/P_Stun_Stars_Base.P_Stun_Stars_Base"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_UNDAMAGED(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_Heal_Shrine_Start.P_Heal_Shrine_Start"));

	/*if (P_DAMAGED.Succeeded())
	{
		Effect->SetTemplate(P_DAMAGED.Object);
		Effect->bAutoActivate = false;
	
	}*/

	if (P_DAMAGED.Succeeded())
	{
		DamagedEffect = P_DAMAGED.Object;
	}
	if (P_UNDAMAGED.Succeeded())
	{
		UndamagedEffect = P_UNDAMAGED.Object;
	}
}

void AHamsterDemoProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	//피직스에 부딪히면 add impulse & destroy
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation()); 

		Effect->SetTemplate(DamagedEffect);

		Effect->Activate(true);
		

		//Destroy();
	}
	else //피직스 없는 액터들
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Effect->SetTemplate(UndamagedEffect);
		
		Effect->Activate(true);
		//Destroy();
	}
}