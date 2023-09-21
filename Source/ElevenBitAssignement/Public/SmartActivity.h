// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeedsAdvertisement.h"
#include "SmartActivity.generated.h"

class AAIGuyCharacter;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnActivityCompleted, const TArray<FNeedAdvertisement>&, NeedsIncrement);

UCLASS()
class ELEVENBITASSIGNEMENT_API ASmartActivity : public AActor
{
	GENERATED_BODY()

public:	
	ASmartActivity();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void LockActivity();
	UFUNCTION(BlueprintCallable)
	void UnlockActivity();
	UFUNCTION(BlueprintCallable)
	FVector GetActivitySpot();

	//Core
	void Perform(AAIGuyCharacter* Performer);

	//Components
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* ActivitySpot;

	UPROPERTY(EditAnywhere) // this is probably bad, given we allow everyone to change it, but comes in handy for the designers to prototype
	bool bIsAvailable = true;

	UPROPERTY(EditAnywhere)
	float CompletionTime = 2.0f;
	UPROPERTY(EditAnywhere)
	FName UniqueIdentifier;
	UPROPERTY(EditAnywhere)
	UAnimationAsset* ActivityAnimation;
	UPROPERTY(EditAnywhere)
	TArray<ASmartActivity*> ChainedActivities;

	UPROPERTY(EditAnywhere)
	UActivityAdvertisements* AdvertisementsDataAsset;
	UPROPERTY(EditAnywhere)
	TArray<FNeedAdvertisement> ActivityAdvertisements;

	// Delegate to notify that an activity was completed - fired once the CompletionTime is reached.
	FOnActivityCompleted OnActivityCompleted;

private:
	float ElapsedTime;
	bool bIsBeingPerformed = false;
};
