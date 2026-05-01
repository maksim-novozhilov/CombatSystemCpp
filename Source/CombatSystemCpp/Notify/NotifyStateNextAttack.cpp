// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyStateNextAttack.h"
#include "Class_Kynan.h"
#include "Interface_Weapon.h"

void UNotifyStateNextAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	//тут пеедаем тру в функцию из интерфеса

	if (MeshComp && MeshComp->GetOwner())
	{
		if (AClass_Kynan* Character = Cast<AClass_Kynan>(MeshComp->GetOwner()))
		{
			if (Character->GetCurrentWeapon())
			{
				AActor* GetWeapon = Character->GetCurrentWeapon();

		
				IInterface_Weapon::Execute_NextAttack(GetWeapon, true);

			}

			
		}


	}
}



void UNotifyStateNextAttack::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	//тут мы передаем фалс в функцию из интереса
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AClass_Kynan* Character = Cast<AClass_Kynan>(MeshComp->GetOwner()))
		{
			if (Character->GetCurrentWeapon())
			{
				AActor* GetWeapon = Character->GetCurrentWeapon();


				IInterface_Weapon::Execute_NextAttack(GetWeapon, false);

			}


		}


	}

}
