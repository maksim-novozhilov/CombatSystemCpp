// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyDropWeapon.h"
#include "Class_Kynan.h"
#include "Interface_Weapon.h"

void UNotifyDropWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AClass_Kynan* Character = Cast<AClass_Kynan>(MeshComp->GetOwner()))
		{
			if (Character->GetCurrentWeapon())
			{
				AActor* GetWeapon = Character->GetCurrentWeapon();

				IInterface_Weapon::Execute_StartDropWeapon(GetWeapon);

				
			}
		}
	}
}
