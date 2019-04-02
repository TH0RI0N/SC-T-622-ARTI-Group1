// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanoidSensorsComponent.h"
#include "Components/ActorComponent.h"
#include "BossAgentComponent.generated.h"

UENUM(BlueprintType)
enum class EBossActionEnum : uint8
{
	BE_AttackSimple		UMETA(DisplayName = "Simple Attack"),
	BE_AttackSpin		UMETA(DisplayName = "Spinning Attack"),
	BE_AttackCombo2		UMETA(DisplayName = "2 Attack Combo"),
	BE_AttackCombo3		UMETA(DisplayName = "3 Attack Combo"),
	BE_Wait				UMETA(DisplayName = "Wait"),
	BE_MoveForward		UMETA(DisplayName = "Move forward"),
	BE_ChargeForward	UMETA(DisplayName = "Charge Forward"),
	BE_ChargeAttack		UMETA(DisplayName = "Charge Attack"),
	BE_Idle				UMETA(DisplayName = "Idle"),
	BE_Dead				UMETA(DisplayName = "Dead")
};

// NOTE: Changes in the numbers of the enum values must be updated here !
namespace EnumSize
{
	const int TARGET_POSITIONS = 4;
	const int TARGET_DISTANCES = 4;
	const int TARGET_ACTIONS = 6;
	const int BOSS_ACTIONS = 7;
}


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSFIGHT_API UBossAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossAgentComponent();

	// A value which represents how likely the agent is to pick an action even though it hasn't
	// provided good results in the past. This is the base value used for giving the action a probability.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplorationValue;

	// A value which represents how likely the agent is to pick an action that provided results in the past.
	// Every time the action is successful in this particular state, this value is added to the total. The
	// value is then divided with the number of times the action has been performed, yielding an average
	// value representing how good results come from performing that specific action in that state.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExploitationValue;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// This lookup table contains the information of the boss agent.
	// The indices are as follows:
	// 0 - Player position
	// 1 - Player distance
	// 2 - Player action
	// 3 - Performed boss action
	// 4 - The data used to calculate the average value of this state
	// 4[0] - The value accumulated for this action in this state
	// 4[1] - The number of times this action has been performed in this state
	float LookUpTable[EnumSize::TARGET_POSITIONS][EnumSize::TARGET_DISTANCES][EnumSize::TARGET_ACTIONS][EnumSize::BOSS_ACTIONS][2];

	float CurrentActionValues[EnumSize::BOSS_ACTIONS];

	EBossActionEnum DecideCourseOfAction(const EPlayerDistanceEnum& DistancePercept);
	float GetActionLength(const EBossActionEnum& Action, const EPlayerDistanceEnum& DistancePercept );

	EBossActionEnum LastBossAction;
	EPlayerActionEnum LastTargetAction;
	EPlayerDistanceEnum LastTargetDistance;
	EPlayerPositionEnum LastTargetPosition;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PostInitProperties() override;
	void InitializeLookUpTable();

	UFUNCTION(BlueprintCallable, Category = "Artificial intelligence")
	void ExecuteAgentFunction(const EPlayerActionEnum& ActionPercept, const EPlayerDistanceEnum& DistancePercept,
		const EPlayerPositionEnum& PositionPercept, const float& Health, const bool& LastAttackSuccessful,
		EBossActionEnum& Action, float& ActionLength );
};