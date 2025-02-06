// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DuelGameMode.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	EGP_Intro UMETA(DisplayName="Intro"),
	EGP_Wait UMETA(DisplayName="Wait"),
	EGP_Draw UMETA(DisplayName="Draw"),
	EGP_Finish UMETA(DisplayName="Finish"),


	EGS_MAX UMETA(DisplayName="Number of Game phases")
};

DECLARE_MULTICAST_DELEGATE(FOnDrawTimerFinished);

UCLASS()
class QUICKDRAW_API ADuelGameMode : public AGameModeBase
{
	GENERATED_BODY()
	// ============================
	// AActor
	// ============================
public:
	virtual void BeginPlay() override;

	// ============================
	// AGameModeBase
	// ============================
public:
	virtual void StartPlay() override;
	virtual void SetPlayerDefaults(APawn* PlayerPawn) override;

	// ============================
	// ADuelGameMode
	// ============================	
public:
	ADuelGameMode();
	void StartDuel();
	void SetDuelGamePhase(EGamePhase NewPhase);
	
	FOnDrawTimerFinished OnDrawTimerFinished;
	
private:
	UPROPERTY(VisibleAnywhere, Category="Duel")
	EGamePhase GamePhase{EGamePhase::EGP_Intro};
	
	// ============================
	// ADuelGameMode - Intro phase
	// ============================	
public:
	
	// ============================
	// ADuelGameMode - Wait phase
	// ============================	
private:
	void OnWaitPhaseEnd();

	FTimerHandle WaitTimerHandle;

	// ============================
	// ADuelGameMode - Draw phase
	// ============================	
public:
	void StartDraw();
	/**
	 * Draw 페이즈를 강제로 종료
	 * @brief ASamuraiPlayer가 DrawPhase에 칼을 휘둘렀을 경우 ASamurai가 호출
	 */
	void EndDrawPhase();

private:
	void OnDrawPhaseEnd();

	/**
	 * 칼을 휘둘러야 하는 타이밍에 화면 중앙에 표시되는 느낌표
	 * @brief DrawMark로 할당될 Blueprint 에셋
	 */
	UPROPERTY(EditDefaultsOnly, Category="Duel|DrawPhase")
	TSubclassOf<class APaperSpriteActor> ExclamationMark;
	/**
	 * 실제 World에 Actor로 생성된 느낌표
	 * @brief ExclamationMark 블루프린트 에셋으로 생성됨
	 */
	TObjectPtr<APaperSpriteActor> DrawMark;
	FTimerHandle DrawTimerHandle;
	/**
	 * 플레이어가 Draw를 실행할 수 있는 시간
	 */
	UPROPERTY(EditDefaultsOnly, Category="Duel|DrawPhase")
	float ValidDrawTime{.8f};

	// ============================
	// ADuelGameMode - Finish phase
	// ============================	
public:
	/**
	 * Finish 페이즈를 시작합니다.
	 */
	void StartFinishPhase();
	virtual void ResetLevel() override final;

private:
	// ============================
	// ADuelGameMode - Inline Getter & Setter
	// ============================	
public:
	FORCEINLINE EGamePhase GetDuelGamePhase() const { return GamePhase; }
};
