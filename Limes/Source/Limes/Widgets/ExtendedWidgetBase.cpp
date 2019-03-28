// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtendedWidgetBase.h"

void UExtendedWidgetBase::Show()
{
	SetRenderOpacity(0);
	SetVisibility(ESlateVisibility::Visible);
	OnShow();


}

void UExtendedWidgetBase::Hide()
{
	OnHide();
	SetVisibility(ESlateVisibility::Collapsed);


}




