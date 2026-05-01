// Fill out your copyright notice in the Description page of Project Settings.


#include "Class_Kynan.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Interface_Weapon.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AClass_Kynan::AClass_Kynan()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}

	if (GetMesh())
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
	
	
	bIsSprinting = false;
	/*SprintSpeed = 750.f;
	DefaultWalkSpeed = 450.f;*/

	bJustLanded = false;

}



// Called when the game starts or when spawned
void AClass_Kynan::BeginPlay()
{
	Super::BeginPlay();

	SetupInputMappingContext();
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;

	///////////////// Подключение Start and End OVERLAP
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AClass_Kynan::OnWeaponOverlapStart);
		GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AClass_Kynan::OnWeaponOverlapEnd);
	}
	
	/////////////Создаем в инвентаре 2 пустые ясейки
	WeaponInventory.Init(nullptr, 2);
	
	
}

// Called every frame
void AClass_Kynan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//SETUP INPUT MAPPING CONTEXT
void AClass_Kynan::SetupInputMappingContext()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
			if (KynanMappingContext)
				{
					Subsystem->AddMappingContext(KynanMappingContext,0);
				}
			}
		}
}







void AClass_Kynan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Движение wasd
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &AClass_Kynan::MoveForward);
		EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &AClass_Kynan::MoveRight);

		//Мышь, камера
		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AClass_Kynan::Turn);
		EnhancedInputComponent->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &AClass_Kynan::LookUp);

		//Прыжок
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AClass_Kynan::Jumping);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Спринт
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AClass_Kynan::SprintStart);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AClass_Kynan::SprintStop);

		
		//Атака ЛКМ
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &AClass_Kynan::AttackLBM);

		//Е подбирание оружия
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &AClass_Kynan::Interact);

		//Переключение слота на 1,2
		EnhancedInputComponent->BindAction(IA_OneWeaponSlot, ETriggerEvent::Started, this, &AClass_Kynan::OneWeaponSlot);
		EnhancedInputComponent->BindAction(IA_TwoWeaponSlot, ETriggerEvent::Started, this, &AClass_Kynan::TwoWeaponSlot);

		//Выход из игры
		EnhancedInputComponent->BindAction(IA_ExitGame, ETriggerEvent::Started, this, &AClass_Kynan::ExitGame);
	}
}


void AClass_Kynan::MoveForward(const FInputActionValue& Value)
{
	if (!GetCharacterMovement()->IsFalling() && bJustLanded)
	{
		StopJumpMontage();
	}
	
	// Получаем значение (1.0 для W, -1.0 для S)
	float MovementValue = Value.Get<float>();

	if (Controller && (MovementValue != 0.0f))
	{
		// 1. Берем поворот контроллера
		const FRotator Rotation = Controller->GetControlRotation();
		// 2. Обнуляем наклон (Pitch) и крен (Roll), оставляем только поворот вокруг оси Z (Yaw)
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 3. Получаем вектор "Вперед" (Ось X) относительно этого поворота
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// 4. Двигаем персонажа
		AddMovementInput(ForwardDirection, MovementValue);
	}
}

void AClass_Kynan::MoveRight(const FInputActionValue & Value)
{
	if (!GetCharacterMovement()->IsFalling() && bJustLanded)
	{
		StopJumpMontage();
	}
	
	// Получаем значение (1.0 для W, -1.0 для S)
	float MovementValue = Value.Get<float>();

	if (Controller && (MovementValue != 0.0f))
	{
		// 1. Берем поворот контроллера
		const FRotator Rotation = Controller->GetControlRotation();
		// 2. Обнуляем наклон (Pitch) и крен (Roll), оставляем только поворот вокруг оси Z (Yaw)
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 3. Получаем вектор "Вперед" (Ось X) относительно этого поворота
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 4. Двигаем персонажа
		AddMovementInput(RightDirection, MovementValue);
	}
}

void AClass_Kynan::Turn(const FInputActionValue & Value)
{
	// Получаем значение смещения мыши по горизонтали
	float LookValue = Value.Get<float>();

	if (LookValue != 0.0f)
	{
		// Встроенная функция Unreal для поворота вокруг вертикальной оси (Z)
		AddControllerYawInput(LookValue);
	}
}

void AClass_Kynan::LookUp(const FInputActionValue & Value)
{
	// Получаем значение смещения мыши по горизонтали
	float LookValue = Value.Get<float>();

	if (LookValue != 0.0f)
	{
		// Встроенная функция Unreal для поворота вокруг вертикальной оси (Z)
		AddControllerPitchInput(LookValue);
	}
}

void AClass_Kynan::SprintStart()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AClass_Kynan::SprintStop()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;

}

void AClass_Kynan::Jumping()
{
	Jump();

	IInterface_Weapon::Execute_ResetCombo(EquippedWeapon);

	bJustLanded = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && JumpMontage)
	{
		
		AnimInstance->Montage_Play(JumpMontage);
	}

}



void AClass_Kynan::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bJustLanded = true;

	if (GetVelocity().Size2D() > 10.f)
	{
		StopJumpMontage();
	};
}

void AClass_Kynan::StopJumpMontage()
{
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (AnimInst->Montage_IsPlaying(JumpMontage))
		{
			AnimInst->Montage_Stop(0.2f, JumpMontage);
		}
	}
	bJustLanded = false;
}


void AClass_Kynan::ExitGame()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("Кнопка нажата"));
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}


void AClass_Kynan::AttackLBM()
{
	
	if (EquippedWeapon)
	{
		
		IInterface_Weapon::Execute_Attack(EquippedWeapon, GetMesh());
		
	}
}

void AClass_Kynan::Interact()
{
	
	if (DetectedWeapon)
	{
		WeaponToBack();
			
		bHasWeapon = true;

		EquippedWeapon = DetectedWeapon;

		IInterface_Weapon::Execute_FlyToHand(EquippedWeapon, GetMesh());
			
		SaveWeaponToArray();

		return;
	}

	else if (bHasWeapon && EquippedWeapon)
	{
		IInterface_Weapon::Execute_DropWeapon(EquippedWeapon, GetMesh());
	}
	
}

void AClass_Kynan::WeaponToBack()
{
	if (EquippedWeapon)
	{
		IInterface_Weapon::Execute_WeaponMoveToBack(EquippedWeapon, GetMesh());
	}
}

void AClass_Kynan::WeaponToHand()
{
	IInterface_Weapon::Execute_WeaponMoveToHand(EquippedWeapon, GetMesh());
}

void AClass_Kynan::SaveWeaponToArray()
{
	for (int32 i = 0; i < WeaponInventory.Num(); i++)
	{
		if (WeaponInventory[i] == nullptr)
		{
			WeaponInventory[i] = EquippedWeapon;

			ActiveSlotIndex = i;

			return;
		}
	}	
}



void AClass_Kynan::OneWeaponSlot()
{
	SwitchWeapon(0);
}

void AClass_Kynan::TwoWeaponSlot()
{
	SwitchWeapon(1);
}


void AClass_Kynan::SwitchWeapon(int32 PressedSlotIndex)
{
	if (ActiveSlotIndex == PressedSlotIndex)//если мы хотим убрать оружие
	{
		WeaponToBack();

		EquippedWeapon = nullptr;

		ActiveSlotIndex = -1;

		if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
		{
			IInterface_Weapon::Execute_UpdateWeaponType(AnimInst, EWeaponType::EWT_None);
		}
		
		return;
	}

	if (WeaponInventory[PressedSlotIndex] != nullptr)// если мы хотим достать оружие
	{
		
		if (EquippedWeapon != nullptr)//проверка, не находится ли сейчас в руке оружие, если да, то активное оружие сначала пермести на спину
		{
			WeaponToBack();
		}

		EquippedWeapon = WeaponInventory[PressedSlotIndex];
		ActiveSlotIndex = PressedSlotIndex;
		WeaponToHand();

	}


}


void AClass_Kynan::ClearPickingUpWeapon_Implementation()
{
	if (EquippedWeapon)
	{
		if (ActiveSlotIndex != -1)
		{
			WeaponInventory[ActiveSlotIndex] = nullptr;
		}
		ActiveSlotIndex = -1;

		bool bFoundSomething = false;

		for (int32 i = 0; i < WeaponInventory.Num(); i++)//цикл, если инвентарь пустой, то выключаем переменную bHasWeapon
		{
			if (WeaponInventory[i])
			{
				bFoundSomething = true;
				break;
			}

		}

		bHasWeapon = bFoundSomething;
	}
	
	
	EquippedWeapon = nullptr;
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("ПЕРСОНАЖ: Ссылка очищена!"));
	//bHasWeapon = false;
}

void AClass_Kynan::OnWeaponOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->Implements<UInterface_Weapon>())
		{
			DetectedWeapon = OtherActor;

			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
					FString::Printf(TEXT("Оружие в зоне доступа: %s"), *OtherActor->GetName()));
			}*/
		}
	}

}

void AClass_Kynan::OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && DetectedWeapon == OtherActor)
	{
		DetectedWeapon = nullptr;
		
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Оружие потеряно"));
		}*/
	}
}


