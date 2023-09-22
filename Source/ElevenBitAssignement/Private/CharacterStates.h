#pragma once

#include "CoreMinimal.h"
#include "AIGuyCharacter.h"
#include "SmartActivity.h"

class AAIGuyCharacter;

//storage class to keep track of used states
class FCharacterStateNames
{
public:
	inline static const FName Idle = FName(TEXT("Idle"));
	inline static const FName MovingToActivity = FName(TEXT("MovingToActivity"));
	inline static const FName PerformingActivity = FName(TEXT("PerformingActivity"));
};

// State machine base class
class FAIStateBase
{
public:
	FAIStateBase(const FName Name) : StateName(Name){}
	virtual ~FAIStateBase() {} 
	virtual void EnterState(AAIGuyCharacter* Owner) {}
	virtual void UpdateState(AAIGuyCharacter* Owner, float DeltaTime) {}
	virtual void ExitState(AAIGuyCharacter* Owner) {}

	virtual FName GetStateName()
	{
		return StateName;
	}

protected:
	FName StateName;
};

/////////////// Derived State classes ///////////////////

/**
 * @brief IDLE STATE
 */
class FIdleState : public FAIStateBase
{

public:
	FIdleState(const FName Name) : FAIStateBase(Name){}
	virtual void EnterState(AAIGuyCharacter* Owner) override {}
	virtual void ExitState(AAIGuyCharacter* Owner) override {}

	virtual void UpdateState(AAIGuyCharacter* Owner, float DeltaTime) override;
};

/**
 * @brief MOVING TO ACTIVITY STATE
 */
class FMovingToActivityState : public FAIStateBase
{

public:
	FMovingToActivityState(const FName Name) : FAIStateBase(Name){}
	virtual void EnterState(AAIGuyCharacter* Owner) override {}
	virtual void ExitState(AAIGuyCharacter* Owner) override {}

	virtual void UpdateState(AAIGuyCharacter* Owner, float DeltaTime) override;
};

/**
 * @brief PERFORMING ACTIVITY STATE
 */
class FPerformingActivityState : public FAIStateBase
{
public:
	FPerformingActivityState(const FName Name) : FAIStateBase(Name){}
	virtual void EnterState(AAIGuyCharacter* Owner) override
	{
		check(Owner->SelectedActivity)
		Owner->RegisterActivityCompletionDelegateHelper();
		//block incremented needs decay
		Owner->IterateAdvertisements(Owner->SelectedActivity->ActivityAdvertisements, [this](FCharacterNeed& Need, FNeedAdvertisement Increment)
		{
			Need.bIsPaused = true;
		});
	}

	virtual void ExitState(AAIGuyCharacter* Owner) override {}
	virtual void UpdateState(AAIGuyCharacter* Owner, float DeltaTime) override{}
};