// Fill out your copyright notice in the Description page of Project Settings.


#include "SamuraiPlayer.h"
#include "DuelGameMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraActor.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"

ASamuraiPlayer::ASamuraiPlayer()
{
	Tags.Add(TEXT("SamuraiPlayer"));
}

void ASamuraiPlayer::InitPlayerCamera(const TObjectPtr<APlayerController>& PlayerController) const
{
	if (PlayerController && CharacterCamera)
	{
		CharacterCamera->SetActorLocation(FVector(0.f, 276.f, 0.f));
		CharacterCamera->SetActorRotation(FRotator(0.f, -90.f, 0.f));
		PlayerController->SetViewTarget(CharacterCamera);
	}
}

void ASamuraiPlayer::DrawSword()
{
	Super::DrawSword();

	if (DuelGameMode)
	{
		DuelGameMode->EndDrawPhase();
	}
}

void ASamuraiPlayer::BeginPlay()
{
	Super::BeginPlay();

	CharacterCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass());
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(SamuraiMappingContext, 0);
			}
		}

		InitPlayerCamera(PlayerController);
	}
	
	if (OtherSamurai == nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SamuraiNPC"), FoundActors);
		if (FoundActors.Num() > 0)
		{
			OtherSamurai = Cast<ASamurai>(FoundActors[0]);
		}
	}
}

void ASamuraiPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &ASamuraiPlayer::Attack);
	}
}

void ASamuraiPlayer::Destroyed()
{
	Super::Destroyed();
}

void ASamuraiPlayer::Reset()
{
	bIsDrawExecuted = false;
	bIsValidAttack = false;
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		InitPlayerCamera(PlayerController);
	}

	InitSamurai();

	K2_OnReset();
}

void ASamuraiPlayer::ExecuteDraw()
{
	if (!DuelGameMode || bIsDrawExecuted) return;
	bIsDrawExecuted = true;

	if (DuelGameMode->GetDuelGamePhase() == EGamePhase::EGP_Wait)
	{
		CrossMarkSprite->SetVisibility(true);
	}
	else if (DuelGameMode && DuelGameMode->GetDuelGamePhase() == EGamePhase::EGP_Draw)
	{
		DrawSword();
		bIsValidAttack = true;
	}
}

void ASamuraiPlayer::Attack()
{
	if (!DuelGameMode) return;
	if (DuelGameMode->GetDuelGamePhase() < EGamePhase::EGP_Wait ||
		DuelGameMode->GetDuelGamePhase() > EGamePhase::EGP_Draw) return;
	
	ExecuteDraw();
}
