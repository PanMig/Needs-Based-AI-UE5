#include "CharacterStates.h"
#include "AIController.h"
#include "AIGuyController.h"
#include "Navigation/PathFollowingComponent.h"

void FIdleState::UpdateState(AAIGuyCharacter* Owner, float DeltaTime)
{
	// Try performing queued activities
	if (!Owner->Activities.IsEmpty())
	{
	    Owner->Activities.Dequeue(Owner->SelectedActivity);
		check(Owner->SelectedActivity)
		Owner->SetNavigationPoint(Owner->SelectedActivity->GetActivitySpot());
		Owner->ChangeState(new FMovingToActivityState(FCharacterStateNames::MovingToActivity));
	}
	else
	{
		//Find our next activity and lock it temporarily
	    ASmartActivity* NextActivity = Owner->SelectNextActivity(true);
		if(!NextActivity)
		{
			UE_LOG(LogTemp, Warning, TEXT("No activity found"));
			return;
		}
	    Owner->Activities.Enqueue(NextActivity);
		NextActivity->LockActivity();
	}
}

void FMovingToActivityState::UpdateState(AAIGuyCharacter* Owner, float DeltaTime)
{
	check(Owner->SelectedActivity);
	if(Owner->AIGuyController->MoveToLocation(Owner->GetNavigationPoint(), 5) == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		Owner->SelectedActivity->Perform(Owner);
		Owner->ChangeState(new FPerformingActivityState(FCharacterStateNames::PerformingActivity));
	}
	else if(Owner->AIGuyController->MoveToLocation(Owner->GetNavigationPoint(), 5) == EPathFollowingRequestResult::Type::Failed)
	{
		Owner->SelectedActivity->UnlockActivity();
		Owner->SelectedActivity = nullptr;
		Owner->Activities.Empty();
		Owner->ChangeState(new FIdleState(FCharacterStateNames::Idle));
		UE_LOG(LogTemp, Warning, TEXT("Path failed, returning to idle"));
	}
}
