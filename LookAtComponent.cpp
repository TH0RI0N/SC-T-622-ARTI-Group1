// Fill out your copyright notice in the Description page of Project Settings.

#include "LookAtComponent.h"

// Sets default values for this component's properties
ULookAtComponent::ULookAtComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	BodyRotationLimitYaw = 60;
	BodyRotationSpeed = 30;
	HeadRotationLimitRoll = 90;
	HeadRotationLimitYaw = 140;
	HeadRotationSpeed = 60;
	RotationGoalTolerance = 3;
	NeutralAngle = 30;
	FacingSpeed = 2;
}


// Called when the game starts
void ULookAtComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULookAtComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULookAtComponent::CalculateRotation(FRotator& BodyRotation, FRotator& HeadRotation, FRotator& CharacterDeltaRotation, const bool& Facing, const bool& CanSeeTarget, const FVector& TargetPosition, const FVector& HeadPosition, const FRotator& CharacterRotation, const float& DeltaSeconds )
{
	// Calculate the rotation of the head
	FVector DirVector = FVector(HeadPosition);
	DirVector -= TargetPosition;
	FRotator FromHeadToTarget = DirVector.Rotation();

	FRotator DeltaRotation = FRotator::ZeroRotator;
	float TargetYaw = FromHeadToTarget.Yaw;

	if (TargetYaw > 360)
	{
		TargetYaw -= 360;
	}
	else if (TargetYaw < 0)
	{
		TargetYaw += 360;
	}

	// Facing

	if (!Facing) {
		CharacterDeltaRotation = CharacterRotation;
	}
	else {
		CharacterDeltaRotation = FRotator::ZeroRotator;
		CharacterDeltaRotation.Yaw = TargetYaw + 90;
		CharacterDeltaRotation.Clamp();
		CharacterDeltaRotation = FMath::Lerp(CharacterRotation, CharacterDeltaRotation, 0.05);
	}

	if (!CanSeeTarget) {
		return;
	}

	// Body rotation

	float Angle = CharacterRotation.Yaw + CurrentBodyRotation.Yaw - 90 + NeutralAngle;
	if (Angle > 360)
	{
		Angle -= 360;
	}
	else if (Angle < 0)
	{
		Angle += 360;
	}

	if ( ( Angle > TargetYaw + RotationGoalTolerance ) || ( Angle < 45 && TargetYaw > 315 ) )
	{
		CurrentBodyRotation.Yaw -= BodyRotationSpeed * DeltaSeconds;
		if (CurrentBodyRotation.Yaw < -BodyRotationLimitYaw / 2)
		{
			CurrentBodyRotation.Yaw = -BodyRotationLimitYaw / 2;
		}
	}
	else if ( ( Angle < TargetYaw - RotationGoalTolerance ) || (TargetYaw < 45 && Angle > 315 ) )
	{
		CurrentBodyRotation.Yaw += BodyRotationSpeed * DeltaSeconds;
		if (CurrentBodyRotation.Yaw > BodyRotationLimitYaw / 2)
		{
			CurrentBodyRotation.Yaw = BodyRotationLimitYaw / 2;
		}
	}

	BodyRotation.Yaw = NeutralAngle + CurrentBodyRotation.Yaw;

	// Head rotation

	Angle = CharacterRotation.Yaw + CurrentHeadRotation.Yaw - 90 + NeutralAngle;
	if (Angle > 360) {
		Angle -= 360;
	}
	else if (Angle < 0 ) {
		Angle += 360;
	}

	if ( ( Angle > TargetYaw + RotationGoalTolerance ) || (Angle < 45 && TargetYaw > 315 ))
	{
		CurrentHeadRotation.Yaw -= HeadRotationSpeed * DeltaSeconds;
		if (CurrentHeadRotation.Yaw < -HeadRotationLimitYaw / 2)
		{
			CurrentHeadRotation.Yaw = -HeadRotationLimitYaw / 2;
		}
	}
	else if ( ( Angle < TargetYaw - RotationGoalTolerance ) || (TargetYaw < 45 && Angle > 315 ) )
	{
		CurrentHeadRotation.Yaw += HeadRotationSpeed * DeltaSeconds;
		if (CurrentHeadRotation.Yaw > HeadRotationLimitYaw / 2)
		{
			CurrentHeadRotation.Yaw = HeadRotationLimitYaw / 2;
		}
	}

	if ( CanSeeTarget && CurrentHeadRotation.Roll > FromHeadToTarget.Pitch + RotationGoalTolerance)
	{
		CurrentHeadRotation.Roll -= HeadRotationSpeed * DeltaSeconds;
		if (CurrentHeadRotation.Roll < -HeadRotationLimitRoll / 2)
		{
			CurrentHeadRotation.Roll = -HeadRotationLimitRoll / 2;
		}
	}
	else if ( CanSeeTarget && CurrentHeadRotation.Roll < FromHeadToTarget.Pitch - RotationGoalTolerance)
	{
		CurrentHeadRotation.Roll += HeadRotationSpeed * DeltaSeconds;
		if (CurrentHeadRotation.Roll > HeadRotationLimitRoll / 2)
		{
			CurrentHeadRotation.Roll = HeadRotationLimitRoll / 2;
		}
	}

	HeadRotation.Roll = CurrentHeadRotation.Roll + 90;
	HeadRotation.Yaw = CurrentHeadRotation.Yaw + NeutralAngle;
}

