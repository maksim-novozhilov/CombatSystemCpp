// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimMontage.h"
#include "Interface_Weapon.h"
#include "Engine/EngineTypes.h"


#include "AClass_WeaponBase.generated.h"







UCLASS()
class COMBATSYSTEMCPP_API AAClass_WeaponBase : public AActor, public IInterface_Weapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAClass_WeaponBase();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* GripPoint;


	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponDamage")
	float DamageWeapon;
//public: FORCEINLINE float GetDamageWeapon() const { return DamageWeapon;}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponMontage")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponMontage")
	UAnimMontage* PickUpWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponMontage")
	UAnimMontage* DropWeapon;


	virtual void FlyToHand_Implementation(USceneComponent* HandMesh) override;
	virtual void StartFlyToHand_Implementation() override;
	virtual void DropWeapon_Implementation(USceneComponent* HandMesh) override;
	virtual void StartDropWeapon_Implementation() override;
	virtual void Attack_Implementation(USceneComponent* MeshKynan) override;
	virtual void NextAttack_Implementation(bool bCanNextAttack) override;
	virtual void ResetCombo_Implementation() override;
	virtual void WeaponMoveToBack_Implementation(USceneComponent* MeshKynan) override;
	virtual void WeaponMoveToHand_Implementation(USceneComponent* MeshKynan) override;
	virtual void WeaponDamageCollision_Implementation(bool EnableCollision) override;

	

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName HandSocketName;
	//SwordRightHandSocket
	//AxeRightHandSocket
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName BackSocketName;
	//AxeBackSocket
	//SwordBackSocket
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FlySpeed;

	float CurrentFlySpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float AccelerationRate = 2000.0f;


	bool bIsCanFly = false;
	bool bIsCanDrop = false;
	bool bCanNextAttackWeapon = false;
	bool bIsAttaking = false;
	int32 ComboIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 ResetComboIndex;

	UPROPERTY()
	bool bIsFalling;
	
	UPROPERTY()
	class USceneComponent* TargetMesh;

	class ACharacter* KynanCharacter;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
