// Fill out your copyright notice in the Description page of Project Settings.


#include "Samurai.h"
#include "PaperSpriteComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DuelGameMode.h"

ASamurai::ASamurai()
{
	PrimaryActorTick.bCanEverTick = false;
	CharacterSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(FName(TEXT("CharacterSprite")));
	SetRootComponent(CharacterSpriteComponent);
	CrossMarkSprite = CreateDefaultSubobject<UPaperSpriteComponent>(FName(TEXT("CrossMarkSprite")));
	CrossMarkSprite->SetupAttachment(GetRootComponent());

	SlideInTimeline = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("SlideInTimeline")));
	DrawSwordInTimeline = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("DrawSwordInTimeline")));
}

void ASamurai::InitSamurai()
{
	SetActorLocation(SpawnLocation);
	
	if (IdleSprite && CharacterSpriteComponent) CharacterSpriteComponent->SetSprite(IdleSprite);
	if (CrossMarkSprite) CrossMarkSprite->SetVisibility(false);
	
	bIsDefeated = false;
	SetSlideInTimeline();
}

void ASamurai::BeginPlay()
{
	Super::BeginPlay();

	DuelGameMode = Cast<ADuelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	InitSamurai();
}

void ASamurai::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASamurai::Reset()
{
	Super::Reset();

	InitSamurai();
}

void ASamurai::Duel()
{
	ChangeToDrawSprite();
	
	if (OtherSamurai)
	{
		OtherSamurai->Dead();
	}
}

void ASamurai::ChangeCharacterSprite(UPaperSprite* NewSprite) const
{
	if (CharacterSpriteComponent && NewSprite)
	{
		CharacterSpriteComponent->SetSprite(NewSprite);
	}
}

void ASamurai::DrawSword()
{
	SetDrawSwordTimeline();
}

void ASamurai::Dead()
{
	bIsDefeated = true;
	if (CrossMarkSprite) CrossMarkSprite->SetVisibility(false);
	ChangeToDeadSprite();
}

void ASamurai::SlideIn(float InProgress)
{
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, InProgress);
	SetActorLocation(NewLocation);
}

void ASamurai::ChangeToDrawSprite() const
{
	ChangeCharacterSprite(DrawSprite);
}

void ASamurai::SetSlideInTimeline()
{
	TargetLocation = GetActorLocation();
	StartLocation = TargetLocation;
	StartLocation.X -= SlideLength.X;
	StartLocation.Z -= SlideLength.Y;
	
	if (SlideInTimeline && SlideInCurve){
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("SlideIn"));
		SlideInTimeline->AddInterpFloat(SlideInCurve, TimelineProgress);
		
		SlideInTimeline->SetLooping(false);
		SlideInTimeline->SetPlayRate(SlideInSpeed);
		SlideInTimeline->PlayFromStart();
	}
}

void ASamurai::DrawSwordIn(float InProgress)
{
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, InProgress);
	SetActorLocation(NewLocation);
}

void ASamurai::SetDrawSwordTimeline()
{
	StartLocation = GetActorLocation();
	TargetLocation.X = StartLocation.X + DrawSwordLength.X;
	TargetLocation.Z = StartLocation.Z + DrawSwordLength.Y;

	if (DrawSwordInTimeline && DrawSwordInCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("DrawSwordIn"));
		DrawSwordInTimeline->AddInterpFloat(DrawSwordInCurve, TimelineProgress, FName("THIS IS PROPERTY NAME"), FName("THIS IS TRACK NAME"));
		
		FOnTimelineEvent DuelEvent;
		DuelEvent.BindUFunction(this, FName("Duel"));
		DrawSwordInTimeline->AddEvent(DrawSpriteChangeTime, DuelEvent);
		
		DrawSwordInTimeline->SetLooping(false);
		DrawSwordInTimeline->SetPlayRate(DrawSwordInSpeed);
		DrawSwordInTimeline->PlayFromStart();
	}
}

void ASamurai::ChangeToDeadSprite() const
{
	ChangeCharacterSprite(DeadSprite);
}

