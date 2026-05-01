// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyStDamageCollision.h"
#include "Interface_Weapon.h"
#include "Class_Kynan.h"


void UNotifyStDamageCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AClass_Kynan* Character = Cast<AClass_Kynan>(MeshComp->GetOwner()))
		{
			if (Character)
			{
				AActor* CurrentWeapon = Character->GetCurrentWeapon();

				if (CurrentWeapon)
				{
					IInterface_Weapon::Execute_WeaponDamageCollision(CurrentWeapon, true);
					
				}
			}
		}
	}
}

				



void UNotifyStDamageCollision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference & EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AClass_Kynan* Character = Cast<AClass_Kynan>(MeshComp->GetOwner()))
		{
			if (Character)
			{
				AActor* CurrentWeapon = Character->GetCurrentWeapon();

				if (CurrentWeapon)
				{
					IInterface_Weapon::Execute_WeaponDamageCollision(CurrentWeapon, false);
				}
			}
		}
	}
}

void UNotifyStDamageCollision::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference & EventReference)
{}