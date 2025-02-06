// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGameMode.h"
#include "PaperSpriteActor.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ADuelGameMode::ADuelGameMode()
{
	SetDuelGamePhase(EGamePhase::EGP_Intro);
}

void ADuelGameMode::StartPlay()
{
	Super::StartPlay();

	StartDuel();
}

void ADuelGameMode::SetPlayerDefaults(APawn* PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);
}

void ADuelGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController && PlayerController->GetPawn() == nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SamuraiPlayer"), FoundActors);
		if (FoundActors.Num() > 0)
		{
			TObjectPtr<APawn> SamuraiPlayer = CastChecked<APawn>(FoundActors[0]);
			PlayerController->Possess(SamuraiPlayer);
		}
	}
}

void ADuelGameMode::StartDuel()
{
	SetDuelGamePhase(EGamePhase::EGP_Wait);
	GetWorldTimerManager().SetTimer(WaitTimerHandle, this, &ADuelGameMode::OnWaitPhaseEnd, FMath::RandRange(2.f, 5.f),
	                                false);
}

void ADuelGameMode::SetDuelGamePhase(EGamePhase NewPhase)
{
	if (GamePhase == NewPhase) return;

	GamePhase = NewPhase;
}

void ADuelGameMode::OnWaitPhaseEnd()
{
	SetDuelGamePhase(EGamePhase::EGP_Draw);
	GetWorldTimerManager().ClearTimer(WaitTimerHandle);
	StartDraw();
}

void ADuelGameMode::StartDraw()
{
	if (DrawMark == nullptr)
	{
		DrawMark = GetWorld()->SpawnActor<APaperSpriteActor>(ExclamationMark);
	}

	DrawMark->GetRenderComponent()->SetVisibility(true);

	GetWorldTimerManager().SetTimer(DrawTimerHandle, this, &ADuelGameMode::OnDrawPhaseEnd, ValidDrawTime, false);
}

void ADuelGameMode::EndDrawPhase()
{
	OnDrawPhaseEnd();
}

void ADuelGameMode::OnDrawPhaseEnd()
{
	if (DrawMark)
	{
		DrawMark->GetRenderComponent()->SetVisibility(false);
	}

	// DrawTimerHandle이 종료되지 않고 EndDrawPhase로 강제로 Draw 페이즈가 종료되었을 경우
	if (GetWorldTimerManager().GetTimerRemaining(DrawTimerHandle) > 0.f)
	{
		// 타이머 일시 정지
		GetWorldTimerManager().PauseTimer(DrawTimerHandle);
	}
	else
	{
		// Toad가 공격하도록 Broadcast
		OnDrawTimerFinished.Broadcast();
	}

	StartFinishPhase();
}

void ADuelGameMode::StartFinishPhase()
{
	SetDuelGamePhase(EGamePhase::EGP_Finish);

	// 플레이어가 ValidDrawTime동안  
	if (GetWorldTimerManager().TimerExists(DrawTimerHandle))
	{
		// Draw를 수행하여 타이머가 일시정지된 상태일 경우
		GetWorldTimerManager().ClearTimer(DrawTimerHandle);
#if UE_BUILD_DEVELOPMENT
		UE_LOG(LogTemp, Warning, TEXT("Tanuki won!"));
#endif
	}
	else
	{
		// Draw를 수행하지 못했을 경우 플레이어가 패배
#if UE_BUILD_DEVELOPMENT
		UE_LOG(LogTemp, Warning, TEXT("Tanuki lost!"));
#endif
	}

	FTimerHandle ResetHandle;
	GetWorldTimerManager().SetTimer(ResetHandle, this, &ADuelGameMode::ResetLevel, 3.f, false);
}

void ADuelGameMode::ResetLevel()
{
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* A = *It;
		if (IsValid(A) && A != this && !A->IsA<AController>() && ShouldReset(A))
		{
			A->Reset();
		}
	}

	Reset();

	StartPlay();
}
