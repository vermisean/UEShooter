// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UEShooterGameMode.h"
#include "UEShooterHUD.h"
#include "UEShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUEShooterGameMode::AUEShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUEShooterHUD::StaticClass();
}
