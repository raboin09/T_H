// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHeroStatusWidget.h"

void UBaseHeroStatusWidget::SetupHeroStatusWidget(FCharacterStatusWidgetDataStruct InStatusData)
{
	StatusData = InStatusData;
	OnHeroStatusWidgetSet();
}

FCharacterStatusWidgetDataStruct UBaseHeroStatusWidget::GetCharacterStatusWidgetData() const
{
	return StatusData;
}
