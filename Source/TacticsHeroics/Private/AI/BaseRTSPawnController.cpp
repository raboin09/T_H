#include "BaseRTSPawnController.h"
#include "Actors/BaseCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


void ABaseRTSPawnController::FindTargetInAcquisitionRadius()
{
	TArray<struct FHitResult> HitResults;

	TraceSphere(GetPawn()->GetActorLocation(), AcquisitionRange, GetPawn(), ECC_Pawn, HitResults);

	for (auto& HitResult : HitResults)
	{
		if (HitResult.Actor == nullptr)
		{
			continue;
		}

		if (HitResult.Actor == GetPawn())
		{
			continue;
		}

		auto MyActor = GetPawn();
		auto TargetActor = HitResult.Actor.Get();

		if (MyActor && TargetActor)
		{
			ABaseCharacter* MyChar = Cast<ABaseCharacter>(MyActor);
			ABaseCharacter* TargetChar = Cast<ABaseCharacter>(TargetActor);

			if (MyChar->GetOwnerTeam() == TargetChar->GetOwnerTeam()) {
				continue;
			}
			  
		}

		auto AttackableComponent = HitResult.Actor->FindComponentByClass<UAbilitySystemComponent>();

		if (!AttackableComponent)
		{
			continue;
		}

		// Acquire target.
		Blackboard->SetValueAsObject(TEXT("TargetActor"), HitResult.Actor.Get());

		return;
	}
}

bool ABaseRTSPawnController::HasOrder(EOrderType OrderType) const
{
	return Blackboard->GetValueAsEnum(TEXT("OrderType")) == (uint8)OrderType;
}

bool ABaseRTSPawnController::IsIdle() const
{
	return HasOrder(EOrderType::IDLE);
}

void ABaseRTSPawnController::IssueAttackOrder(AActor* Target)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	SetOrder(EOrderType::ATTACK);
	ClearHomeLocation();
	SetTargetActor(Target);
	ClearTargetLocation();

	ApplyOrders();
}

void ABaseRTSPawnController::IssueMoveOrder(const FVector& Location)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	SetOrder(EOrderType::MOVE);
	ClearHomeLocation();
	ClearTargetActor();
	SetTargetLocation(Location);

	ApplyOrders();
}

void ABaseRTSPawnController::IssueStopOrder()
{
	if (!VerifyBlackboard())
	{
		return;
	}

	SetOrder(EOrderType::IDLE);
	SetHomeLocation(GetPawn()->GetActorLocation());
	ClearTargetActor();
	ClearTargetLocation();

	ApplyOrders();
}

void ABaseRTSPawnController::ApplyOrders()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->RestartTree();
	}

	uint8 NewOrder = Blackboard->GetValueAsEnum(TEXT("OrderType"));
	OnOrderChanged.Broadcast((EOrderType)NewOrder);
}

void ABaseRTSPawnController::ClearHomeLocation()
{
	Blackboard->ClearValue(TEXT("HomeLocation"));
}

void ABaseRTSPawnController::ClearTargetActor()
{
	Blackboard->ClearValue(TEXT("TargetActor"));
}

void ABaseRTSPawnController::ClearTargetLocation()
{
	Blackboard->ClearValue(TEXT("TargetLocation"));
}

void ABaseRTSPawnController::SetHomeLocation(const FVector& HomeLocation)
{
	Blackboard->SetValueAsVector(TEXT("HomeLocation"), HomeLocation);
}

void ABaseRTSPawnController::SetOrder(const EOrderType InOrder)
{
	Blackboard->SetValueAsEnum(TEXT("OrderType"), (uint8)InOrder);
}

void ABaseRTSPawnController::SetTargetActor(AActor* TargetActor)
{
	Blackboard->SetValueAsObject(TEXT("TargetActor"), TargetActor);
}

void ABaseRTSPawnController::SetTargetLocation(const FVector& TargetLocation)
{
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), TargetLocation);
}

bool ABaseRTSPawnController::TraceSphere(const FVector& Location, const float Radius, AActor* ActorToIgnore, ECollisionChannel TraceChannel, TArray<struct FHitResult>& OutHitResults)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	const FVector Start = Location;
	const FVector End = Location + FVector::ForwardVector * Radius;

	return World->SweepMultiByObjectType(
		OutHitResults,
		Start,
		End,
		FQuat(),
		FCollisionObjectQueryParams(TraceChannel),
		FCollisionShape::MakeSphere(Radius)
	);
}

bool ABaseRTSPawnController::VerifyBlackboard()
{
	if (!Blackboard)
	{
		return false;
	}

	return true;
}
