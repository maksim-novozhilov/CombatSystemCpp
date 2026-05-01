// Fill out your copyright notice in the Description page of Project Settings.


#include "KynanAnimInstance.h"
#include "GameFramework/Character.h"
#include "AClass_WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UKynanAnimInstance::NativeInitializeAnimation()
{
Super :: NativeInitializeAnimation();

	KynanCharacter = Cast<ACharacter>(TryGetPawnOwner());

}

void UKynanAnimInstance::NativeUpdateAnimation(float DeltaSecends)
{
Super :: NativeUpdateAnimation(DeltaSecends);

	if (KynanCharacter)
	{
		SpeedXY = KynanCharacter->GetVelocity().Size2D();

		bIsFalling = KynanCharacter->GetCharacterMovement()->IsFalling();
	}

}

void UKynanAnimInstance::UpdateWeaponType_Implementation(EWeaponType NewType)
{
	// Записываем пришедшее значение в нашу переменную
	CurrentWeaponType = NewType;

	// Добавь этот принт, чтобы увидеть в игре, что тип реально сменился
	/*if (GEngine)
	{
		FString TypeName = (NewType == EWeaponType::EWT_Sword) ? TEXT("МЕЧ") : (NewType == EWeaponType::EWT_Axe ? TEXT("ТОПОР") : TEXT("КУЛАКИ"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("AnimInstance: Вижу оружие -> %s"), *TypeName));
	}*/
}
