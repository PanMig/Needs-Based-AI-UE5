// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartActivity.h"
#include "AIGuyCharacter.h"
#include "NavigationSystem.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASmartActivity::ASmartActivity()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBoxComponent"));
	BoxComponent->SetupAttachment(StaticMeshComponent);

	ActivitySpot = CreateDefaultSubobject<UArrowComponent>(TEXT("ActivitySpot"));
	ActivitySpot->SetupAttachment(StaticMeshComponent);

	// Calculate the desired relative transform for ActivitySpot
	const FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // You can change the rotation if needed
	const FTransform RelativeTransform = FTransform(Rotation, FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));

	// Apply the relative transform to ActivitySpot
	ActivitySpot->SetRelativeTransform(RelativeTransform);
}

// Called when the game starts or when spawned
void ASmartActivity::BeginPlay()
{
	Super::BeginPlay();

	//init ActivityAdvertisements
	if(!ActivityAdvertisements.IsEmpty())
	{
		ActivityAdvertisements.Empty();
	}

	check(AdvertisementsDataAsset)
	for(const auto& Advertisement : AdvertisementsDataAsset->NeedAdvertisements)
	{
		ActivityAdvertisements.Add(FNeedAdvertisement(Advertisement.Name, Advertisement.IncrementAmount));
	}
}

// Called every frame
void ASmartActivity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsBeingPerformed && ElapsedTime <= CompletionTime)
	{
		ElapsedTime += DeltaTime;
	}
	else if(ElapsedTime > CompletionTime)
	{
		ElapsedTime = 0.0f;
		bIsBeingPerformed = false;
		OnActivityCompleted.ExecuteIfBound(ActivityAdvertisements);
	}
}

void ASmartActivity::LockActivity()
{
	bIsAvailable = false;
}

void ASmartActivity::UnlockActivity()
{
	bIsAvailable = true;
}

FVector ASmartActivity::GetActivitySpot()
{
	return ActivitySpot->GetComponentLocation();
}

void ASmartActivity::Perform(AAIGuyCharacter* Performer)
{
	UE_LOG(LogTemp, Warning, TEXT("Activity Started"));
	bIsBeingPerformed = true;
	check(Performer);
	if(ActivityAnimation)
	{
		Performer->GetMesh()->PlayAnimation(ActivityAnimation, true);
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("Activity animation failed to play, check if it is assigned."));
}

