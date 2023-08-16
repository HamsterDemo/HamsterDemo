// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//MeshMonster = GetMesh();


	static ConstructorHelpers::FObjectFinder<UAnimSequence> DEAD_ANIM(TEXT("/Game/AnimStarterPack/Death_1.Death_1"));
	if (DEAD_ANIM.Succeeded())
	{
		DeadAnimation = DEAD_ANIM.Object;
	}

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Log, TEXT("Monster TakeDamage"));

	SetHP(MonsterHP, FinalDamage);

	return 0.0f;
}

float AMonsterCharacter::SetHP(float HP, float FinalDamage)
{
	if (MonsterHP == -9999)
		return 0;

	MonsterHP = MonsterHP - FinalDamage;
	UE_LOG(LogTemp, Log, TEXT("Damage %f, Monster HP %f"), FinalDamage, MonsterHP);

	if (MonsterHP <= 0)
	{
		MonsterHP = -9999;

		UE_LOG(LogTemp, Log, TEXT("Monster HP 0 :: Dead"));

		bool bLoop = false;
		GetMesh()->PlayAnimation(DeadAnimation, bLoop);

		FTimerHandle WaitHandle;
		float WaitTime = 5.0;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				Destroy();

			}), WaitTime, false);
		
		return 0;
	}
	
	return 0.0f;
}

//void AMonsterCharacter::PlayDeadAnim()
//{
//	if (DeadAnimation != nullptr)
//	{
//		// Get the animation object for the arms mesh
//		UAnimInstance* AnimInstance = MeshMonster->GetAnimInstance();
//		if (AnimInstance != nullptr)
//		{
//			AnimInstance->Montage_Play(DeadAnimation, 1.f);
//
//		}
//
//
//	}
//}



