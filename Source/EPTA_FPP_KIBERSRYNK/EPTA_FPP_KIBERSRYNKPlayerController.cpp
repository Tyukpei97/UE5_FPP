// Copyright Epic Games, Inc. All Rights Reserved.


#include "EPTA_FPP_KIBERSRYNKPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "EPTA_FPP_KIBERSRYNKCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "EPTA_FPP_KIBERSRYNK.h"
#include "Widgets/Input/SVirtualJoystick.h"

AEPTA_FPP_KIBERSRYNKPlayerController::AEPTA_FPP_KIBERSRYNKPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AEPTA_FPP_KIBERSRYNKCameraManager::StaticClass();
}

void AEPTA_FPP_KIBERSRYNKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogEPTA_FPP_KIBERSRYNK, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AEPTA_FPP_KIBERSRYNKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
	
}

bool AEPTA_FPP_KIBERSRYNKPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
