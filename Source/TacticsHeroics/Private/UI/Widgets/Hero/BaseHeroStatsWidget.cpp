// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHeroStatsWidget.h"

void UBaseHeroStatsWidget::SetupHeroStatsWidget(FCharacterStatsWidgetDataStruct InStatsData)
{
	StatsData = InStatsData;
	OnHeroStatsWidgetSet();
}

FCharacterStatsWidgetDataStruct UBaseHeroStatsWidget::GetCharacterStatsWidgetData() const
{
	return StatsData;
}
