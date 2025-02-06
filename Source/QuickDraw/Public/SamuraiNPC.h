// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Samurai.h"
#include "SamuraiNPC.generated.h"

UCLASS()
class QUICKDRAW_API ASamuraiNPC : public ASamurai
{
	GENERATED_BODY()
// ============================
// AActor
// ============================
public:
	virtual void BeginPlay() override;

// ============================
// ASamuraiNPC
// ============================
public:
	ASamuraiNPC();
	
private:
	void OnDrawTimerFinished();
};
