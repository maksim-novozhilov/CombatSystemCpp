// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface_Weapon.h"
#include "KynanAnimInstance.generated.h"

/**
 * 
 */


UCLASS()
class COMBATSYSTEMCPP_API UKynanAnimInstance : public UAnimInstance, public IInterface_Weapon
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSecends) override;

	class ACharacter* KynanCharacter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpeedXY")
	float SpeedXY;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "bIsFalling")
	bool bIsFalling;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	 EWeaponType CurrentWeaponType = EWeaponType::EWT_None;

virtual void UpdateWeaponType_Implementation(EWeaponType NewType) override;

	
};
