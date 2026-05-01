// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyResetCombo.h"
#include "Class_Kynan.h"
#include "Interface_Weapon.h"

void UNotifyResetCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (AClass_Kynan* Character = Cast<AClass_Kynan>(MeshComp->GetOwner()))
		{
			if (Character->GetCurrentWeapon())
			{
				AActor* GetWeapon = Character->GetCurrentWeapon();


				IInterface_Weapon::Execute_ResetCombo(GetWeapon);

			}
		}
	}
}
