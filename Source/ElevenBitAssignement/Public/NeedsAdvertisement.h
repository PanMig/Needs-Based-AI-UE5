// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NeedsAdvertisement.generated.h"

USTRUCT(BlueprintType)
struct FCharacterNeed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Need")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Need")
	float Weight = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Need")
	float MaxValue = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Need")
	float Value = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Need")
	float FullDecayInSeconds = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Need")
	bool bIsPaused = false;

	FCharacterNeed ()= default;

	FCharacterNeed(const FString& Name, float Weight, float MaxValue, float Value, float FullDecayInSeconds)
		: Name(Name),
		  Weight(Weight),
		  MaxValue(MaxValue),
		  Value(Value),
		  FullDecayInSeconds(FullDecayInSeconds){}

	float CalculateScore(const float FutureValue)
	{
		// Calculate A(current) and A(future) using the formula A(x) = 10/x
		const float ACurrent = (Value != 0.0f) ? 10.0f / Value : 0.0f;
	    const float AFuture = (FutureValue != 0.0f) ? 10.0f / FutureValue : 0.0f;

		// Calculate the weighted score using the single weight
		const float WeightedScore = Weight * (ACurrent - AFuture);

	    return WeightedScore;
	}
};

USTRUCT(BlueprintType)
struct FNeedAdvertisement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IncrementAmount = 0.0f;

	FNeedAdvertisement ()= default;
	FNeedAdvertisement(FString Name, float IncrementAmount) : Name(Name), IncrementAmount(IncrementAmount){}
};

UCLASS()
class ELEVENBITASSIGNEMENT_API UAICharacterNeeds : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FCharacterNeed> CharacterNeeds;
};

UCLASS()
class ELEVENBITASSIGNEMENT_API UActivityAdvertisements : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FNeedAdvertisement> NeedAdvertisements;
};
