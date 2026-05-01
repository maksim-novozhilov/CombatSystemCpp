// Fill out your copyright notice in the Description page of Project Settings.


#include "KynanGameMode.h"

//AKynanGameMode::AKynanGameMode() : Super()
//{
//	/*ConstructionHelpers::FClassFinder<APawn>MainCharacterKynan(TEXT("/Game/BP/MyClass_Kynan"));
//
//	DefaulPawnClass = MainCharacterKynan;
//	*/
//}

AKynanGameMode::AKynanGameMode()
{
	ConstructorHelpers::FClassFinder<APawn>MainCharacterKunan(TEXT("/Game/BP/CBP_MyClass_Kynan"));
	DefaultPawnClass = MainCharacterKunan.Class;
}
