// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Interface_Weapon.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventaryChanged, int32, SlotIndex, class UTexture2D*, NewIcon)

#include "Class_Kynan.generated.h"



UCLASS()
class COMBATSYSTEMCPP_API AClass_Kynan : public ACharacter, public IInterface_Weapon
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AClass_Kynan();
	
	
	
	//SETUP INPUT MAPPING CONTEXT
	void SetupInputMappingContext();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputMappingContext")
	UInputMappingContext* KynanMappingContext;


	//INPUTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_MoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_MoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Turn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_LookUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_OneWeaponSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_TwoWeaponSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_ExitGame;

	//Для прыжка
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages_Animations")
	UAnimMontage* JumpMontage;

	bool bJustLanded;

	void Landed(const FHitResult& Hit);

	void StopJumpMontage();



	//для бега
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Sprint;

	bool bIsSprinting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DefaultWalkSpeed;


	
	//кнопки для аттаки оружием и интеракта
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Interact;
	
	//Функции для оверлапа
	UFUNCTION()
	void OnWeaponOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	



	//input functions
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(const struct FInputActionValue& Value);
	void MoveRight(const struct FInputActionValue& Value);
	void Turn(const struct FInputActionValue& Value);
	void LookUp(const struct FInputActionValue& Value);
	void SprintStart();
	void SprintStop();
	void Jumping();
	void AttackLBM();
	void Interact();
	void OneWeaponSlot();
	void TwoWeaponSlot();
	void ExitGame();
	
	//функция из интерфейа для очистки ссылки на оружие
	virtual void ClearPickingUpWeapon_Implementation() override;


//функция гетер, для передачи текущего оружия в нотифай
public: class AActor* GetCurrentWeapon() const { return EquippedWeapon; }



//переменная обнаружения оружия под ногами
protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	AActor* DetectedWeapon;
//переменная которая хранит оружие в руках 
protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	AActor* EquippedWeapon; 
//переменная, персонаж имеет какое то оружие
protected: bool bHasWeapon = false;
//массив переменная инвентарь оружия	
protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	TArray<AActor*> WeaponInventory;
//эта переменная указывает на слот в инвентаре, оружие из которого находится в руках
protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
		 int32 ActiveSlotIndex = -1;	

		UPROPERTY(BlueprintAssignable)
		FOnInventaryChanged OnInventaryChanged;

//функция, которая ищет первый свободный слот в инвентаре, записывает туда найденное оружие
protected: UFUNCTION()
		 void SaveWeaponToArray();
//функция, которая переключает оружие в руке, а так же убирает его за спину
protected: UFUNCTION()
		 void SwitchWeapon(int32 PressedSlotIndex);
//Функция, меняет иконку в инвентаре на экране
protected: UFUNCTION()
		 void SetInventoryIcon();		 

//функция, которая перемешает оружие из руки на спину
protected: UFUNCTION()
		 void WeaponToBack();
//функция, которая перемешает оружие со спины в руку
protected: UFUNCTION()
		 void WeaponToHand();


		 UFUNCTION()
		 void TestPrint(int32 SlotIndex, UTexture2D* NewIcon);





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
