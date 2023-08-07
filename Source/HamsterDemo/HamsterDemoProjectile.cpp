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
}

void AHamsterDemoProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	//�������� �ε����� add impulse & destroy
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation()); 

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
			if (DamagedEffect != nullptr)
			{
				World->SpawnActor<AProjectileEffect>(DamagedEffect, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}

			if (OtherActor) //���߿� Damagable üũ
			{
				FDamageEvent DamageEvent;
				OtherActor->TakeDamage(50.0, DamageEvent, CharacterController, this);
			}

			// ���� �� > �Ѿ��� ��򰡿� �¾���(�Ȼ������) > ���� ��ü�� �����ͼ� > ���� ��ü�� �������ִ¾ָ� > �������� �� 
			// OnFire > OnHit > HitResult.GetActor > if(Damagable) > HitResult�� TakeDamage ȣ��

		}
	}

	Destroy();

}

AController* AHamsterDemoProjectile::SetCharacterController(AController* Controller)
{
	CharacterController = Controller;
	return CharacterController;
}

