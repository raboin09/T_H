
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Types/RTSTypes.h"
#include "BaseRTSPawnController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSAIControllerEventDelegate, EOrderType, InOrder);

UCLASS()
class TACTICSHEROICS_API ABaseRTSPawnController : public AAIController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void FindTargetInAcquisitionRadius();

	UFUNCTION(BlueprintPure)
		bool HasOrder(EOrderType OrderType) const;

	UFUNCTION(BlueprintPure)
		bool IsIdle() const;

	UFUNCTION(BlueprintCallable)
		void IssueAttackOrder(AActor* Target);

	UFUNCTION(BlueprintCallable)
		void IssueMoveOrder(const FVector& Location);

	UFUNCTION(BlueprintCallable)
		void IssueStopOrder();

private:

	void ApplyOrders();

	void ClearHomeLocation();
	void ClearTargetActor();
	void ClearTargetLocation();

	void SetHomeLocation(const FVector& HomeLocation);
	void SetOrder(const EOrderType InOrder);
	void SetTargetActor(AActor* TargetActor);
	void SetTargetLocation(const FVector& TargetLocation);

	bool TraceSphere(const FVector& Location, const float Radius, AActor* ActorToIgnore, ECollisionChannel TraceChannel, TArray<struct FHitResult>& OutHitResults);
	bool VerifyBlackboard();

public:

	UPROPERTY(BlueprintAssignable, Category = "RTS_AI")
		FRTSAIControllerEventDelegate OnOrderChanged;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RTS_AI")
		UBehaviorTree* CharacterBehaviorTreeAsset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RTS_AI")
		UBlackboardData* CharacterBlackboardAsset;

	UPROPERTY(BlueprintReadOnly)
		float AcquisitionRange = 1000.f;
};
