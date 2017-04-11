// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RPGProject.h"
#include "RPGProjectGameMode.h"
#include "RPGProjectCharacter.h"

ARPGProjectGameMode::ARPGProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_RPGProjectCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ARPGProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	ChangeMenuWidget(StaminaWidgetClass);
}

void ARPGProjectGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}