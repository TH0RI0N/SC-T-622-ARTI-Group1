// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAgentComponent.h"

// Sets default values for this component's properties
UBossAgentComponent::UBossAgentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ExploitationValue = 1;
	ExplorationValue = 2;
	// ...
}

void UBossAgentComponent::PostInitProperties()
{
	Super::PostInitProperties();
	InitializeLookUpTable();
}


// Called when the game starts
void UBossAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBossAgentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBossAgentComponent::ExecuteAgentFunction(const EPlayerActionEnum& ActionPercept, const EPlayerDistanceEnum& DistancePercept,
	const EPlayerPositionEnum& PositionPercept, const float& Health, const bool& LastAttackSuccessful,
	EBossActionEnum& Action, float& ActionLength)
{

	if (LastAttackSuccessful)
	{
		LookUpTable[(int)LastTargetPosition][(int)LastTargetDistance]
				   [(int)LastTargetAction][(int)LastBossAction][0] += ExploitationValue;
	}

	LastTargetPosition = PositionPercept;
	LastTargetDistance = DistancePercept;
	LastTargetAction = ActionPercept;

	// Idle if the player is dead
	if ( ActionPercept == EPlayerActionEnum::PE_Dead ) 
	{
		Action = EBossActionEnum::BE_Idle;
		LastBossAction = Action;
		ActionLength = 5;
		return;
	}

	// Do the spinning attack to reorient if the player is not in sight
	if (ActionPercept == EPlayerActionEnum::PE_Unknown)
	{
		Action = EBossActionEnum::BE_AttackSpin;
		LastBossAction = Action;
		ActionLength = GetActionLength(Action, DistancePercept );
		return;
	}

	Action = DecideCourseOfAction( DistancePercept );
	LastBossAction = Action;
	ActionLength = GetActionLength(Action, DistancePercept );
}

void UBossAgentComponent::InitializeLookUpTable() 
{

	// Initialize look up table to zeroes
	for (int pos = 0; pos < EnumSize::TARGET_POSITIONS; pos++)
	{
		for (int dist = 0; dist < EnumSize::TARGET_DISTANCES; dist++)
		{
			for (int targetAction = 0; targetAction < EnumSize::TARGET_ACTIONS; targetAction++)
			{
				for (int bossAction = 0; bossAction < EnumSize::BOSS_ACTIONS; bossAction++)
				{
					LookUpTable[pos][dist][targetAction][bossAction][0] = 0;
					// Set the amount of actions to 0.01 to avoid divide by zero errors
					LookUpTable[pos][dist][targetAction][bossAction][1] = 0.01;
				}
			}
		}
	}
}

EBossActionEnum UBossAgentComponent::DecideCourseOfAction( const EPlayerDistanceEnum& DistancePercept )
{
	float value;
	float totalValue = 0;

	int lowerBound = 0;
	int upperBound = 3;

	if (DistancePercept == EPlayerDistanceEnum::PE_Medium)
	{
		lowerBound = 4;
		upperBound = 5;
	}
	else if (DistancePercept == EPlayerDistanceEnum::PE_Far)
	{
		lowerBound = 5;
		upperBound = 6;
	}

	// Calculate the relative probabilities of each action and the total value
	for (int BossAction = lowerBound; BossAction <= upperBound; BossAction++)
	{
		// Don't do the same attack twice in a row
		if (((EBossActionEnum)BossAction) == LastBossAction)
		{
			continue;
		}

		value = LookUpTable[(int)LastTargetPosition][(int)LastTargetDistance]
			[(int)LastTargetAction][ BossAction ][0];
		value /= LookUpTable[(int)LastTargetPosition][(int)LastTargetDistance]
			[(int)LastTargetAction][BossAction][1];
		value += ExplorationValue;

		CurrentActionValues[BossAction] = value;
		totalValue += value;
	}

	float rnd = FMath::FRandRange(0, totalValue);

	// Pick from the actions weighted by the previously calculated value
	for (int BossAction = lowerBound; BossAction <= upperBound; BossAction++)
	{
		// Don't do the same attack twice in a row
		if (((EBossActionEnum)BossAction) == LastBossAction)
		{
			continue;
		}

		if (rnd <= CurrentActionValues[BossAction])
		{
			return (EBossActionEnum) BossAction;
		}

		rnd -= CurrentActionValues[BossAction];
	}

	return EBossActionEnum::BE_Idle;
}

float UBossAgentComponent::GetActionLength(const EBossActionEnum& Action, const EPlayerDistanceEnum& DistancePercept )
{
	switch (Action)
	{
	case EBossActionEnum::BE_AttackSimple:
		return 1.84;
	case EBossActionEnum::BE_AttackCombo2:
		return 2.73;
	case EBossActionEnum::BE_AttackCombo3:
		return 4.2;
	case EBossActionEnum::BE_AttackSpin:
		return 3.16;
	case EBossActionEnum::BE_Wait:
		return DistancePercept == EPlayerDistanceEnum::PE_Near 
			? FMath::FRandRange(0.15, 0.3) : FMath::FRandRange(0.25, 0.35);
	case EBossActionEnum::BE_MoveForward:
		return DistancePercept == EPlayerDistanceEnum::PE_Far
			? FMath::FRandRange(1, 2) : FMath::FRandRange(0.2, 0.5);
	case EBossActionEnum::BE_ChargeForward:
		return FMath::FRandRange(1.5, 2.5);
	}

	return 0.5;
}
