// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIArenaTickableWorldSubsystem.generated.h"

class AAIGuyCharacter;
class ASmartActivity;

UCLASS(BlueprintType)
class NEEDSBASEDAI_API UAIArenaTickableWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UAIArenaTickableWorldSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual TStatId GetStatId() const override { return TStatId(); }
	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	bool FindAvailableActivityWithID(const FName& ID, ASmartActivity*& FoundActivity);

	UFUNCTION(BlueprintCallable)
	TArray<ASmartActivity*> GetAvailableSmartActivities();
	UFUNCTION(BlueprintCallable)
	void BroadcastForcedActivity(FName ID);

	UPROPERTY()
	TArray<AAIGuyCharacter*> AICharacters;
	UPROPERTY()
	TArray<ASmartActivity*> SmartActivities;
	UPROPERTY(EditAnywhere)
	float ElapsedTime = 0.0f;
};
