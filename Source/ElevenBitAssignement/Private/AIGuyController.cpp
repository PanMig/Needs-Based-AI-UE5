// Fill out your copyright notice in the Description page of Project Settings.

#include "Navigation/PathFollowingComponent.h"
#include "AIGuyController.h"

void AAIGuyController::BeginPlay()
{
	Super::BeginPlay();

    OnMoveCompleted.AddDynamic(this, &AAIGuyController::HandleOnMoveCompleted);
}

void AAIGuyController::HandleOnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
    {
	    UE_LOG(LogTemp, Warning, TEXT("Reached goal"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Reaching goal"));
    }
}
