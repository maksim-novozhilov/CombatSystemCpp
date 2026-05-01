// Fill out your copyright notice in the Description page of Project Settings.


#include "AClass_WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"


#include "Components/PrimitiveComponent.h"


// Sets default values
AAClass_WeaponBase::AAClass_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMEsh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	
	// 1. Физика
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 2. Очистка (ставим Ignore на ВСЁ, чтобы потом включить только нужное)
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	// 3. Блокируем ПОЛ (WorldDynamic) и стены (WorldStatic)
	WeaponMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	// 4. Игнорируем игрока и камеру
	//WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	//WeaponMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// 5. Защита от "прошивания" пола
	if (WeaponMesh->GetBodyInstance())
	{
		WeaponMesh->GetBodyInstance()->bUseCCD = true;
	}

	


	
	

	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(WeaponMesh);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(WeaponMesh);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore); //Игнорируем все каналы(чтобы не цепляться за пол и стены)
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //Реагируем только на персонажей (Pawn)


	GripPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GripPint"));
	GripPoint->SetupAttachment(WeaponMesh);



	bIsFalling = false;
}





void AAClass_WeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Overlap called"));
	if (OtherActor && OtherActor != this)
	{
		
		AActor* OverlappedActor = OtherActor;
		
		
		if (OverlappedActor->Implements<UInterface_Weapon>())
		{
			IInterface_Weapon::Execute_GetDamageEnemy(OverlappedActor, DamageWeapon);
			
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("актор передался"));
		}
	}
		
}
		
		

void AAClass_WeaponBase::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}





void AAClass_WeaponBase::FlyToHand_Implementation(USceneComponent* HandMesh)
{
	if (GEngine)
	{
		/*FString Msg = FString::Printf(TEXT("Я лечу в сокет: %s"), *HandSocketName.ToString());
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Cyan, Msg);*/

		if (HandMesh)
		{
			TargetMesh = HandMesh;

			WeaponMesh->SetSimulatePhysics(false);

			CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
			bIsFalling = true;
			
			KynanCharacter = Cast<ACharacter>(HandMesh->GetOwner());

			CurrentFlySpeed = 0.f;
			
			if (PickUpWeapon && KynanCharacter)
			{
				KynanCharacter->PlayAnimMontage(PickUpWeapon);
			}
			
		}
	}

}

void AAClass_WeaponBase::StartFlyToHand_Implementation()
{
	bIsCanFly = true;
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("НОТИФАЙ СРАБОТАЛ: Разрешаю полет!"));
	}*/

}

void AAClass_WeaponBase::DropWeapon_Implementation(USceneComponent* HandMesh)
{
	if (HandMesh)
	{
		TargetMesh = HandMesh;
		KynanCharacter = Cast<ACharacter>(TargetMesh->GetOwner());

		if (KynanCharacter && DropWeapon)
		{
			KynanCharacter->PlayAnimMontage(DropWeapon);
		}
	}
		
}

void AAClass_WeaponBase::StartDropWeapon_Implementation()
{
	bIsCanDrop = true;
}



void AAClass_WeaponBase::Attack_Implementation(USceneComponent* MeshKynan)
{
	// в этой функции мы запускаем монтаж
	ACharacter* Character = Cast<ACharacter>(MeshKynan->GetOwner());

	

	if (!Character) return;
	
	if (!bIsAttaking)
	{
		
		bIsAttaking = true;
		ComboIndex = 0;
		Character->PlayAnimMontage(AttackMontages[ComboIndex]);
		
	

		
		ComboIndex++;
		return;
	}

	if (bIsAttaking && bCanNextAttackWeapon)
	{
		if (ComboIndex >= AttackMontages.Num())
		{
			ComboIndex = ResetComboIndex;
		}

		Character->PlayAnimMontage(AttackMontages[ComboIndex]);
		
		/*if (GEngine)
		{
			FString Msg = FString::Printf(TEXT("УДАР №: %d"), ComboIndex);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, Msg);
		}*/

		
		ComboIndex++;
		bCanNextAttackWeapon = false;
	}
}


void AAClass_WeaponBase::NextAttack_Implementation(bool bCanNextAttack)
{
	//в этой функции срабатывает переменная которая разрешает, переключение на следущую атаку

	bCanNextAttackWeapon = bCanNextAttack;
}

void AAClass_WeaponBase::ResetCombo_Implementation()
{
	bIsAttaking = false;
	ComboIndex = 0;
}

void AAClass_WeaponBase::WeaponMoveToBack_Implementation(USceneComponent* MeshKynan)
{
	if (MeshKynan)
	{
		TargetMesh = MeshKynan;

		if (TargetMesh)
		{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		this->AttachToComponent(TargetMesh, AttachRules, BackSocketName);
		}
	}
}

void AAClass_WeaponBase::WeaponMoveToHand_Implementation(USceneComponent* MeshKynan)
{
	if (MeshKynan)
	{
		TargetMesh = MeshKynan;

		if (TargetMesh)
		{
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
			this->AttachToComponent(TargetMesh, AttachRules, HandSocketName);
		}

		if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(TargetMesh))
		{

			if (UAnimInstance* AnimInst = SkeletalMesh->GetAnimInstance())
			{

				IInterface_Weapon::Execute_UpdateWeaponType(AnimInst, WeaponType);
			}
		}
	}
}

//Функция включения коллизии во время атаки
void AAClass_WeaponBase::WeaponDamageCollision_Implementation(bool EnableCollision)
{
	if (EnableCollision)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Коллизия вкл"));
		}*/

	}

	if (!EnableCollision)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Коллизия выкл"));
		}*/
	}
}

			
			

// Called when the game starts or when spawned
void AAClass_WeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAClass_WeaponBase::OnOverlapBegin);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AAClass_WeaponBase::OnOverlapEnd);
	}
	
}

// Called every frame
void AAClass_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////////////////////////Перемещение оружия в руку и прикрепление его к руке////////////////////////////////////////////////////////
	

	if (bIsFalling && TargetMesh && bIsCanFly)
	{
		FVector TargetLoc = TargetMesh->GetSocketLocation(HandSocketName);
		FRotator TargetRot = TargetMesh->GetSocketRotation(HandSocketName);

		FVector CurrentGripPoint = GripPoint->GetComponentLocation();

		float Distance = FVector::Dist(CurrentGripPoint, TargetLoc);

		float AcceptableDistance = 5.0f;

		/*if (AActor* OwnerActor = TargetMesh->GetOwner())
		{
			float CharacterSpeedXY = OwnerActor->GetVelocity().Size2D();

			if (CharacterSpeedXY > 10.f)
			{
				AcceptableDistance = 25.f;
			}
		}*/
		
		if (Distance < AcceptableDistance)
		{
			// Защелкиваем сокеты

			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
			this->AttachToComponent(TargetMesh, AttachRules, HandSocketName);

			bIsFalling = false;
			bIsCanFly = false;
			
			if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(TargetMesh))
			{
				
				if (UAnimInstance* AnimInst = SkeletalMesh->GetAnimInstance())
				{
					
					IInterface_Weapon::Execute_UpdateWeaponType(AnimInst, WeaponType);
				}
			}
		
		}

		else //полет
		{
		
			CurrentFlySpeed += AccelerationRate * DeltaTime;

			FVector Offset = CurrentGripPoint - GetActorLocation();

			FVector TaretBaseLocation = TargetLoc - Offset;

			FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TaretBaseLocation, DeltaTime, CurrentFlySpeed);

			FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRot, DeltaTime, CurrentFlySpeed);
			
			SetActorLocation(NewLocation);
			SetActorRotation(NewRotation);
			
			
			
			
			
			
			
			
			/*	FVector Offset = CurrentGripPoint - GetActorLocation();
			FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLoc - Offset, DeltaTime, FlySpeed);

			FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, FlySpeed);
			

			SetActorLocation(NewLocation);
			SetActorRotation(NewRotation);*/

		}

	}
	////////////////////////Выбрасывание оружия////////////////////////////////////////////////////////

	else if (TargetMesh && bIsCanDrop)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Оружие откреплено"));
		
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);


		if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(TargetMesh))
		{
			if (UAnimInstance* AnimInst = SkeletalMesh->GetAnimInstance())
			{
				IInterface_Weapon::Execute_UpdateWeaponType(AnimInst, EWeaponType::EWT_None);
			}
		}

		
		
		
		//  Включаем физику меша, чтобы он упал
		if (WeaponMesh)
		{
			WeaponMesh->SetSimulatePhysics(true);
			// Включаем коллизию меша, чтобы он не провалился сквозь пол
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
			WeaponMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			WeaponMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		}

		// Возвращаем коллизию сфере, чтобы его можно было подобрать СНОВА
		if (CollisionSphere)
		{
			CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			// Убедись, что она по-прежнему игнорирует всё, кроме игрока (как в конструкторе)
			CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
			CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		}

		IInterface_Weapon::Execute_ClearPickingUpWeapon(TargetMesh->GetOwner());
		
		bIsCanDrop = false;
		bIsAttaking = false;
		TargetMesh = nullptr;
		
	}
}