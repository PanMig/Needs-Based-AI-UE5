#pragma once

#include "CoreMinimal.h"
#include "NeedsAdvertisement.h"
#include "GameFramework/Character.h"
#include "Containers/Queue.h"
#include "AIGuyCharacter.generated.h"

class ASmartActivity;
class FAIStateBase;

UCLASS()
class ELEVENBITASSIGNEMENT_API AAIGuyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//UE Derived
	AAIGuyCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//CORE
	void NeedsDecay(float DeltaTime);
	ASmartActivity* SelectNextActivity(bool bDebugOn);
	//We will require many times to iterate through the needs and advertisements arrays, so let's generalize it
	void IterateAdvertisements(const TArray<FNeedAdvertisement>& Advertisements, TFunction<void(FCharacterNeed&, const FNeedAdvertisement& Advertisement)> Lambda);

	//Helpers
	FVector GetNavigationPoint() const { return NavigationPoint; }
	void SetNavigationPoint(const FVector& NewPoint) { NavigationPoint = NewPoint; }
	void RegisterActivityCompletionDelegateHelper();
	void ChangeState(FAIStateBase* NewState);
	void ClearActivity();

	//Editor Exposed
	UFUNCTION(BlueprintCallable, Category = "Needs AI")
	void ReceiveActivityReward(const TArray<FNeedAdvertisement>& NeedsIncrement);
	UFUNCTION(BlueprintCallable, Category = "Needs AI")
	void ForceActivity(ASmartActivity* ForcedActivity);
	UFUNCTION(BlueprintPure, Category = "Needs AI")
	FName GetAIStateName();

	UPROPERTY()
	class AAIGuyController* AIGuyController;
	UPROPERTY()
	class UAIArenaTickableWorldSubsystem* WorldSubsystem;
	UPROPERTY()
	ASmartActivity* SelectedActivity;

	//Data asset to load character needs
	UPROPERTY(EditAnywhere, Category = "Needs AI")
	UAICharacterNeeds* CharacterNeedsDataAsset;
	//List of needs that are loaded at runtime (from the DA) to allow for easier operations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs AI")
	TArray<FCharacterNeed> Needs;

	TQueue<ASmartActivity*> Activities;

private:
	FAIStateBase* CurrentState;
	FVector NavigationPoint;
	// let's hard code that, for the math to work
	UE::Math::TVector2<float> NeedsValueRange {1.0f, 100.0f};
};
