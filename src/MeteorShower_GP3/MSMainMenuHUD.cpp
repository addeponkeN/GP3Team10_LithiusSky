// Fill out your copyright notice in the Description page of Project Settings.


#include "MSMainMenuHUD.h"

#include "MenuManager.h"

void AMSMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	MenuManager = NewObject<UMenuManager>(GetTransientPackage(), FName("MainMenuManager"));
}
