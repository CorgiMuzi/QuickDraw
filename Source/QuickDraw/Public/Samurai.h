// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Samurai.generated.h"

class ADuelGameMode;
class UPaperSpriteComponent;
class UPaperSprite;

UCLASS(Abstract)
class QUICKDRAW_API ASamurai : public APawn
{
	GENERATED_BODY()
	// ============================
	// AActor
	// ============================
public:
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	virtual void Reset() override;

	// ============================
	// ASamurai
	// ============================
public:
	ASamurai();
	void InitSamurai();
	
protected:
	void ChangeCharacterSprite(UPaperSprite* NewSprite) const;

	UPROPERTY(VisibleAnywhere, Category="Samurai")
	TObjectPtr<ADuelGameMode> DuelGameMode;
	UPROPERTY(EditDefaultsOnly, Category="Samurai")
	TObjectPtr<UPaperSpriteComponent> CrossMarkSprite{nullptr};
	UPROPERTY(VisibleAnywhere, Category="Samurai")
	TObjectPtr<ASamurai> OtherSamurai{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Samurai")
	FVector SpawnLocation{FVector::Zero()};
	bool bIsDefeated{false};

private:
	/**
	 * 호출한 Samurai는 칼을 휘두르는 스프라이트로, 상대 Samurai는 패배하는 스프라이트로 교체하는 Event 
	 * @details DrawSword()를 호출하면 SetDrawSwordTimeline() 안에서 Duel()을 호출하는 FTimelineEvent를 등록한다.
	 * @attention 캐릭터가 이동하고, 칼을 휘두르는 스프라이트로 교체하고 싶다면 DrawSword()를 호출할것
	 */
	UFUNCTION()
	virtual void Duel();

	UPROPERTY(EditDefaultsOnly, Category="Samurai")
	TObjectPtr<UPaperSpriteComponent> CharacterSpriteComponent{nullptr};
	
	// ============================
	// ASamurai - 애니메이션
	// ============================
private:
	UPROPERTY(EditDefaultsOnly, Category="Samurai|Animation")
	TObjectPtr<UPaperSprite> IdleSprite{nullptr};
	UPROPERTY(VisibleAnywhere, Category="Samurai|Animation")
	FVector StartLocation{FVector::Zero()};
	UPROPERTY(VisibleAnywhere, Category="Samurai|Animation")
	FVector TargetLocation{FVector::Zero()}; // World에 놓여진 위치

	// 게임 시작시 캐릭터가 화면 안으로 들어오는 애니메이션
private:
	void SetSlideInTimeline();

	/**
	 * 캐릭터가 화면 안으로 들어오는 애니메이션
	 * @param InProgress Timeline 진행도 [0, 1]
	 */
	UFUNCTION()
	void SlideIn(float InProgress);

	UPROPERTY(EditDefaultsOnly, Category="Samurai|Animation")
	TObjectPtr<class UTimelineComponent> SlideInTimeline{nullptr};
	UPROPERTY(EditAnywhere, Category="Samurai|Animation")
	TObjectPtr<class UCurveFloat> SlideInCurve{nullptr};
	// FVector TargetLocation{StartLocation.X + SlideLength.X, 0.f, StartLocation.Z = SlideLength.Y}
	UPROPERTY(EditAnywhere, Category="Samurai|Animation")
	FVector2D SlideLength{0.f, 0.f};
	UPROPERTY(EditAnywhere, Category="Samurai|Animation")
	float SlideInSpeed{1.f};

	// 칼을 휘두르는 애니메이션
protected:
	// 칼을 휘두르러 목적지까지 이동 후 공격하는 스프라이트로 교체
	virtual void DrawSword();

private:
	UFUNCTION(Category="Samurai|Animation")
	void DrawSwordIn(float InProgress);
	void ChangeToDrawSprite() const;
	void SetDrawSwordTimeline();

	/**
	 * 칼을 휘두르는 Sprite
	 */
	UPROPERTY(EditDefaultsOnly, Category="Samurai|Animation")
	TObjectPtr<UPaperSprite> DrawSprite{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Samurai|Animation")
	TObjectPtr<UTimelineComponent> DrawSwordInTimeline{nullptr};
	UPROPERTY(EditAnywhere, Category="Samurai|Animation")
	TObjectPtr<UCurveFloat> DrawSwordInCurve{nullptr};
	UPROPERTY(EditAnywhere, Category="Samurai|Animation")
	FVector2D DrawSwordLength{0.f, 0.f};
	/**
	 * 적에게 칼을 휘두르러 이동하는 시간
	 */
	UPROPERTY(EditAnywhere, Category="Samurai|Animation")
	float DrawSwordInSpeed{1.f};
	/**
	 * 칼을 휘두르는 Sprite로 바뀌는 타이밍
	 */
	UPROPERTY(EditAnywhere, Category="Samurai|Animation")
	float DrawSpriteChangeTime{0.8f};

	// 캐릭터가 패배하는 애니메이션
public:
	/**
	 * 캐릭터가 패배한 스프라이트로 교체
	 */
	virtual void Dead();

private:
	UFUNCTION(Category="Samurai|Animation")
	void ChangeToDeadSprite() const;

	UPROPERTY(EditDefaultsOnly, Category="Samurai|Animation")
	TObjectPtr<UPaperSprite> DeadSprite;
};
