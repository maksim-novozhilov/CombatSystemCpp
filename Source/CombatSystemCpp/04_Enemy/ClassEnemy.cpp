// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "AClass_WeaponBase.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBarWidget.h"





// Sets default values
AClassEnemy::AClassEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	EnemyCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("EnemyCollision"));
	RootComponent = EnemyCollision;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(EnemyCollision);

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidgetComp->SetupAttachment(EnemyCollision);
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	Health = MaxHealth;

}

void AClassEnemy::GetDamageEnemy_Implementation(float DamageReceived)
{
	
	if (DamageReceived <= 0.f || Health <= 0.f)
		return;

	
	Health -= DamageReceived;

	if (auto* HealthBar = Cast<UEnemyHealthBarWidget>(HealthWidgetComp->GetUserWidgetObject()))
	{
		HealthBar->UpdateHealthPercent(Health / MaxHealth); 
	}
	
	
	if (Health <= 0.f)
	{
		EnemyDie();
	}

	if (EnemyMesh && Health > 0.f)
	{
		// Резко наклоняем меш 
		FRotator HitRotation = FRotator(FMath::RandRange(-7.f, 7.f), FMath::RandRange(-7.f, 7.f), 0.f);
		EnemyMesh->SetRelativeRotation(HitRotation);
	}

	
}
	
	
	

void AClassEnemy::EnemyDie()
{
	HealthWidgetComp->SetVisibility(false);

	
	EnemyCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	if (EnemyMesh)//включаем физику и добавляем импульс
	{
		
		EnemyMesh->SetSimulatePhysics(true);
		EnemyMesh->SetCollisionProfileName(TEXT("PhysicsActor"));

		
		FVector ImpulseDir = -GetActorRightVector() + FVector(0, 0, 0.5f);

		
		float ImpulseStrength = 300.0f;

		
		EnemyMesh->AddImpulse(ImpulseDir * ImpulseStrength, NAME_None, true);

		EnemyMesh->AddAngularImpulseInDegrees(GetActorRightVector() * 100.0f, NAME_None, true);
	}
}

// Called when the game starts or when spawned
void AClassEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Делаем полоску полной при появлении
	if (auto* HealthBar = Cast<UEnemyHealthBarWidget>(HealthWidgetComp->GetUserWidgetObject()))
	{
		HealthBar->UpdateHealthPercent(1.0f);
	}
		
	
}

// Called every frame
void AClassEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Возвращение в исходную позицию после импульса при уроне
	if (EnemyMesh && !EnemyMesh->GetRelativeRotation().IsNearlyZero() && Health > 0.f)
	{
		FRotator CurrentRot = EnemyMesh->GetRelativeRotation();
		FRotator TargetRot = FRotator::ZeroRotator; // Возврат в 0

		EnemyMesh->SetRelativeRotation(FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 15.f));
	}

}

// Called to bind functionality to input
void AClassEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

