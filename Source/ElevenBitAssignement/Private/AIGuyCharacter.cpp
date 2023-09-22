// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuyCharacter.h"
#include "AIArenaTickableWorldSubsystem.h"
#include "AIController.h"
#include "AIGuyController.h"
#include "CharacterStates.h"
#include "NeedsAdvertisement.h"
#include "SmartActivity.h"

// Sets default values
AAIGuyCharacter::AAIGuyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIGuyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIGuyController = Cast<AAIGuyController>(GetController());
	WorldSubsystem = GetWorld()->GetSubsystem<UAIArenaTickableWorldSubsystem>();

	if(!Needs.IsEmpty())
	{
		Needs.Empty();
	}
	//init Needs array
	check(CharacterNeedsDataAsset)
	for(auto& Need : CharacterNeedsDataAsset->CharacterNeeds)
	{
		Needs.Add(FCharacterNeed(Need.Name, Need.Weight, Need.MaxValue, Need.Value, Need.FullDecayInSeconds));
	}

	//set initial state
	CurrentState = new FIdleState(FName("Idle"));
	CurrentState->EnterState(this);
}

void AAIGuyCharacter::NeedsDecay(float DeltaTime)
{
	for(auto& Need : Needs)
	{
		if(Need.bIsPaused)
		{
			continue;
		}
		const float DecreaseRate = Need.MaxValue / Need.FullDecayInSeconds;
		Need.Value -= DecreaseRate * DeltaTime;
		Need.Value = FMath::Max(Need.Value, NeedsValueRange.X);
	}
}

ASmartActivity* AAIGuyCharacter::SelectNextActivity(bool bDebugOn)
{
	check(WorldSubsystem)

	//Calculate needs score for all activities
	auto SmartActivities = WorldSubsystem->GetAvailableSmartActivities();
	TArray<TPair<ASmartActivity*, float>> ActivityScores;

	/*
	*IMPORTANT: This O(n^3) + early exit solution may look bad, and could be reduced to O(n^2) by using an unordered map
	*to query the needs faster, but with an increase in space complexity.
	*Currently though given that the character needs won't probably surpass a number bigger than 100 we can stick
	*with this approach and refactor later if need be.
	*/
	for(const auto& Activity : SmartActivities)
	{
		float Score = 0.0f;
		for(auto& Need : Needs)
		{
			for(const auto& Advertisement : Activity->ActivityAdvertisements)
			{
				if(Need.Name == Advertisement.Name)
				{
					const float NeedScore = Need.CalculateScore(FMath::Min(Need.Value + Advertisement.IncrementAmount, NeedsValueRange.Y));
					Score += NeedScore;
					break; // early exit since match was found
				}
			}
		}
		ActivityScores.Add(TPair<ASmartActivity*, float>(Activity, Score));
	}

	//Sort based on score
	auto CompareDescending = [](const TPair<ASmartActivity*, float>& A, const TPair<ASmartActivity*, float>& B) -> bool
	{
	    return A.Value > B.Value;
	};

	Algo::Sort(ActivityScores, CompareDescending);

	if(bDebugOn)
	{
		UE_LOG(LogTemp, Warning, TEXT("---------------"));
		for(const auto& Score : ActivityScores)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : %f"), *Score.Key->GetActorNameOrLabel(), Score.Value);
		}
		UE_LOG(LogTemp, Warning, TEXT("---------------"));	
	}

	return ActivityScores.Num() > 0 ? ActivityScores[0].Key : nullptr;
}

void AAIGuyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NeedsDecay(DeltaTime);

	CurrentState->UpdateState(this, DeltaTime);
}

void AAIGuyCharacter::IterateAdvertisements(const TArray<FNeedAdvertisement>& Advertisements, TFunction<void(FCharacterNeed&, const FNeedAdvertisement& Advertisement)> Lambda)
{
	for(const auto& Advertisement : Advertisements)
	{
		if(Advertisement.IncrementAmount <= 0)
		{
			continue;
		}
		for(auto& Need : Needs)
		{
			if(Advertisement.Name == Need.Name)
			{
				Lambda(Need, Advertisement);
				break; // early exit if we have a match
			}
		}
	}
}

//helper class to register ActitivityCompletion delegate from outside classes (e.g state machine)
void AAIGuyCharacter::RegisterActivityCompletionDelegateHelper()
{
	SelectedActivity->OnActivityCompleted.BindDynamic(this, &AAIGuyCharacter::ReceiveActivityReward);
}

void AAIGuyCharacter::ChangeState(FAIStateBase* NewState)
{
	// Cleanup the current state
	if (CurrentState)
	{
	    CurrentState->ExitState(this);
	    delete CurrentState;
	}
	// Assign the new state
	CurrentState = NewState;
	// Enter the new state
	CurrentState->EnterState(this);
}

void AAIGuyCharacter::ClearActivity()
{
	SelectedActivity->OnActivityCompleted.Clear();
	SelectedActivity->UnlockActivity();
	//Stop current animation and return to blueprint
	GetMesh()->Stop();
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

void AAIGuyCharacter::ReceiveActivityReward(const TArray<FNeedAdvertisement>& NeedsIncrement)
{
	//apply reward
	IterateAdvertisements(NeedsIncrement, [this](FCharacterNeed& Need, FNeedAdvertisement Increment)
	{
		Need.Value = FMath::Clamp(Need.Value + Increment.IncrementAmount, NeedsValueRange.X, NeedsValueRange.Y);
		Need.bIsPaused = false;
	});

	ClearActivity();

	//check if we have chained activities and add them to the queue
	for(auto& ChainedActivity : SelectedActivity->ChainedActivities)
	{
		if(ChainedActivity->bIsAvailable)
		{
			Activities.Enqueue(ChainedActivity);
			ChainedActivity->LockActivity();
		}
	}

	ChangeState(new FIdleState(FCharacterStateNames::Idle));
}

void AAIGuyCharacter::ForceActivity(ASmartActivity* ForcedActivity)
{
	if(SelectedActivity)
	{
		ClearActivity();
	}
	if(!ForcedActivity)
	{
		UE_LOG(LogTemp, Error, TEXT("Forced activity was null, aborting..."));
	}
	//clear all pending activities
	Activities.Empty();
	//add forced one
	SelectedActivity = ForcedActivity;
	Activities.Enqueue(SelectedActivity);
	SelectedActivity->LockActivity();
	ChangeState(new FIdleState(FCharacterStateNames::Idle));
}

FName AAIGuyCharacter::GetAIStateName()
{
	return CurrentState->GetStateName();
}