// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/Texture2D.h"
#include "Interface_Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventaryChanged, int32, SlotIndex, class UTexture2D*, NewIcon);




UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_None UMETA(DisplayName = "NoWeapon"),
	EWT_Sword UMETA(DisplayName = "Sword"),
	EWT_Axe UMETA(DisplayName = "Axe")
};



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Weapon : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBATSYSTEMCPP_API IInterface_Weapon
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void Attack(USceneComponent* MeshKynan); //функция для вызова монтажа

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void WeaponDamageCollision(bool EnableCollision);

	//Получение урона для врага
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void GetDamageEnemy(float DamageReceived);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void NextAttack(bool bCanNextAttack); //Функция для обновления переменной разрешения следующей аттаки

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void ResetCombo();// функция для ресета комбо

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void UpdateWeaponType(EWeaponType NewType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void WeaponMoveToBack(USceneComponent* MeshKynan);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void WeaponMoveToHand(USceneComponent* MeshKynan);

	 virtual UTexture2D* GetInventoryIcon() const = 0;


	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void FlyToHand(USceneComponent* HandMesh);//Получаем меш и вызываем монтаж

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void StartFlyToHand();//Функция для обновления переменной (вызывается из нотифая, реализуется в оружии)

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void ClearPickingUpWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void DropWeapon(USceneComponent* HandMesh);//Получаем меш и вызываем монтаж

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void StartDropWeapon();//Функция для обновления переменной (вызывается из нотифая, реализуется в оружии)


};
