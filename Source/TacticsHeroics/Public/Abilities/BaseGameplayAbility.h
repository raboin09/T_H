#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "API/Abilities/BaseGameplayAbilityInterface.h"
#include "BaseGameplayAbility.generated.h"

class UBaseTargetType;

UENUM(BlueprintType)
enum class EGDAbilityInputID : uint8
{
	None            UMETA(DisplayName = "None"),
	Confirm            UMETA(DisplayName = "Confirm"),
	Cancel            UMETA(DisplayName = "Cancel"),
	Ability1        UMETA(DisplayName = "Ability1"),
	Ability2        UMETA(DisplayName = "Ability2"),
	Ability3        UMETA(DisplayName = "Ability3"),
	Ability4        UMETA(DisplayName = "Ability4"),
	Ability5        UMETA(DisplayName = "Ability5"),
	Sprint            UMETA(DisplayName = "Sprint"),
	Jump            UMETA(DisplayName = "Jump")
};

USTRUCT(BlueprintType)
struct FBaseGameplayEffectContainer
{
	GENERATED_BODY()

public:
	FBaseGameplayEffectContainer() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
		TSubclassOf<UBaseTargetType> TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
		TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
};

USTRUCT(BlueprintType)
struct FBaseGameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	FBaseGameplayEffectContainerSpec() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
		FGameplayAbilityTargetDataHandle TargetData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
		TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	bool HasValidEffects() const;

	bool HasValidTargets() const;

	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};

UCLASS()
class TACTICSHEROICS_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBaseGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EGDAbilityInputID AbilityInputID = EGDAbilityInputID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EGDAbilityInputID AbilityID = EGDAbilityInputID::None;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
         TMap<FGameplayTag, FBaseGameplayEffectContainer> EffectContainerMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		 UAnimMontage* MontageToPlay;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
		 FGameplayTag GameplayCueTag;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
		 FGameplayTagContainer MontageEventTags;

    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
         bool ActivateAbilityOnGranted = false;
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
		virtual FBaseGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FBaseGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
		virtual FBaseGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);
	UFUNCTION(BlueprintCallable, Category = Ability)
        virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FBaseGameplayEffectContainerSpec& ContainerSpec);
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
        virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
		virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
