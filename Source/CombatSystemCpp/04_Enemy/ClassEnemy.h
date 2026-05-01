// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface_Weapon.h"
#include "AClass_WeaponBase.h"
#include "Components/WidgetComponent.h"
#include "ClassEnemy.generated.h"

UCLASS()
class COMBATSYSTEMCPP_API AClassEnemy : public APawn, public IInterface_Weapon
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AClassEnemy();

	virtual void GetDamageEnemy_Implementation(float DamageReceived) override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComp;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsEnemy")
	class UStaticMeshComponent* EnemyMesh;//меш
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsEnemy")
	class UCapsuleComponent* EnemyCollision;//коллизя
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsEnemy")
	float MaxHealth;//максимальное здоровье
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	
	float Health;//Текущее здоровье
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsEnemy")
	float AccumulatedDamage = 0.0f;//переменная накопленного урона

	UFUNCTION()
	void EnemyDie();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
