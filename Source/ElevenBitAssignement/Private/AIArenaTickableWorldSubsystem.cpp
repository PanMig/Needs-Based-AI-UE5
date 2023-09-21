// Fill out your copyright notice in the Description page of Project Settings.


#include "AIArenaTickableWorldSubsystem.h"

#include "AIGuyCharacter.h"
#include "SmartActivity.h"
#include "Kismet/GameplayStatics.h"

UAIArenaTickableWorldSubsystem::UAIArenaTickableWorldSubsystem()
{
	AICharacters.Empty();
	SmartActivities.Empty();
}

void UAIArenaTickableWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UAIArenaTickableWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UAIArenaTickableWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
}

void UAIArenaTickableWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	//Find all smart activities available in the World
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASmartActivity::StaticClass(), OutActors);
	Algo::Transform(OutActors, SmartActivities, [](AActor* InActor)
	{
		return Cast<ASmartActivity>(InActor);
	});

	UE_LOG(LogTemp, Warning, TEXT("Total objects: %d"), SmartActivities.Num());

	//Find all AI Characters
	OutActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIGuyCharacter::StaticClass(), OutActors);
	Algo::Transform(OutActors, AICharacters, [](AActor* InActor)
	{
		return Cast<AAIGuyCharacter>(InActor);
	});

	UE_LOG(LogTemp, Warning, TEXT("Total characters: %d"), AICharacters.Num());
}

bool UAIArenaTickableWorldSubsystem::FindAvailableActivityWithID(const FName& ID, ASmartActivity*& FoundActivity)
{
    for (const auto& Activity : SmartActivities)
    {
        if (Activity->bIsAvailable && Activity->UniqueIdentifier == ID)
        {
            FoundActivity = Activity;
            return true;
        }
    }

    // No matching activity was found, set FoundActivity to nullptr to indicate failure.
    FoundActivity = nullptr;
    return false;
}

TArray<ASmartActivity*> UAIArenaTickableWorldSubsystem::GetAvailableSmartActivities()
{
	TArray<ASmartActivity*> AvailableActivities;
	for(auto& SmartAct : SmartActivities)
	{
		if(SmartAct->bIsAvailable)
		{
			AvailableActivities.Add(SmartAct);
		}
	}

	return AvailableActivities;
}

void UAIArenaTickableWorldSubsystem::BroadcastForcedActivity(FName ID)
{
	for(auto& Char : AICharacters)
	{
		ASmartActivity* FoundActivity = nullptr;
		if(FindAvailableActivityWithID(ID, FoundActivity))
		{
			Char->ForceActivity(FoundActivity);
		}
	}
}
