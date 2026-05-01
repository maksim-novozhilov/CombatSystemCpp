// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyStartWeaponFly.h"
#include "Class_Kynan.h"
#include "Interface_Weapon.h"

void UNotifyStartWeaponFly::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AClass_Kynan* Character = Cast<AClass_Kynan>(MeshComp->GetOwner()))
		{
			if (Character->GetCurrentWeapon())
			{
				AActor* GetWeapon = Character->GetCurrentWeapon();

				/*if (GEngine)
				{
					FString WeaponName = GetWeapon->GetName();
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Нотифай нашел оружие: %s"), *WeaponName));
				}*/
				IInterface_Weapon::Execute_StartFlyToHand(GetWeapon);
				
			}

			//else
			//{
			//	// Дополнительная проверка на случай, если GetCurrentWeapon() вернул пустоту
			//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Нотифай: Оружие в персонаже НЕ НАЙДЕНО!"));
			//}
		}
		


	}
}
