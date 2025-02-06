// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Samurai.h"
#include "SamuraiPlayer.generated.h"

/**
 * 
 */
UCLASS()
class QUICKDRAW_API ASamuraiPlayer : public ASamurai
{
	GENERATED_BODY()
	// ============================
	// AActor
	// ============================
public:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Destroyed() override;

	// ============================
	// ASamurai
	// ============================
public:
	virtual void Reset() override;

	// ============================
	// ASamuraiPlayer
	// ============================	
public:
	ASamuraiPlayer();
	void InitPlayerCamera(const TObjectPtr<APlayerController>& PlayerController) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ASamurai> SamuraiNPC{nullptr};

protected:
	virtual void DrawSword() override;

private:
	void ExecuteDraw();

	// 칼을 한번 휘둘렀는가?
	bool bIsDrawExecuted{false};
	// Draw 페이즈일 때 공격했는가?
	bool bIsValidAttack{false};
	UPROPERTY(VisibleAnywhere, Category="Samurai")
	TObjectPtr<class ACameraActor> CharacterCamera{nullptr};

	// ============================
	// ASamuraiPlayer - Enhanced Input
	// ============================
private:
	void Attack();

	UPROPERTY(EditDefaultsOnly, Category="Samurai|Input")
	TObjectPtr<class UInputMappingContext> SamuraiMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="Samurai|Input")
	TObjectPtr<class UInputAction> AttackAction;

	// ============================
	// ASamuraiPlayer - Inline Getter & Setter
	// ============================
public:
	FORCEINLINE bool IsValidAttack() const { return bIsValidAttack; }
};
