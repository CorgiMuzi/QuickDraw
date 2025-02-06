// Fill out your copyright notice in the Description page of Project Settings.


#include "SamuraiNPC.h"
#include "DuelGameMode.h"
#include "Kismet/GameplayStatics.h"

ASamuraiNPC::ASamuraiNPC()
{
	Tags.Add(TEXT("SamuraiNPC"));
}

void ASamuraiNPC::BeginPlay()
{
	Super::BeginPlay();
	
	if (OtherSamurai == nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SamuraiPlayer"), FoundActors);
		if (FoundActors.Num() > 0)
		{
			OtherSamurai = Cast<ASamurai>(FoundActors[0]);
		}
	}

	if (DuelGameMode)
	{
		DuelGameMode->OnDrawTimerFinished.AddUObject(this, &ASamuraiNPC::OnDrawTimerFinished);
	}
}

void ASamuraiNPC::OnDrawTimerFinished()
{
	DrawSword();
}