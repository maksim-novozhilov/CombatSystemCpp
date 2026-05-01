// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBarWidget.h"

void UEnemyHealthBarWidget::UpdateHealthPercent(float NewPercent)
{
    if (HealthBar) 
    {
        HealthBar->SetPercent(NewPercent);
    }
}
