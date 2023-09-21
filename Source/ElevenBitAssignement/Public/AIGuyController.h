// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIGuyController.generated.h"

UCLASS()
class ELEVENBITASSIGNEMENT_API AAIGuyController : public AAIController
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleOnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:
    FAIMoveCompletedSignature OnMoveCompleted;
};
