// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanoidSensorsComponent.h"

// Sets default values for this component's properties
UHumanoidSensorsComponent::UHumanoidSensorsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SightConeAngle = 40;
	ShortRange = 300;
	MediumRange = 500;
	FrontAngle = 30;
	// ...
}


// Called when the game starts
void UHumanoidSensorsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHumanoidSensorsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHumanoidSensorsComponent::PostInitProperties()
{
	Super::PostInitProperties();
	SightConeAngleCosine = FMath::Cos(SightConeAngle);
}

void UHumanoidSensorsComponent::ProducePercepts(const FVector& TargetPosition, const FVector& HeadPosition, const FRotator& CharacterRotation,
	const FVector& GazeDirection, const bool& TargetWalking, const bool& TargetAttacking, const bool& TargetBlocking, const bool& TargetDodging,
	EPlayerActionEnum& TargetAction, EPlayerPositionEnum& RelativeTargetPosition, EPlayerDistanceEnum& RelativeTargetDistance )
{
	FVector DirVector = TargetPosition;
	DirVector -= HeadPosition;

	// If the player is out of sight, return the last known position and unknown action
	if ( DirVector.CosineAngle2D( GazeDirection ) < SightConeAngleCosine ) {
		TargetAction = EPlayerActionEnum::PE_Unknown;
		RelativeTargetPosition = EPlayerPositionEnum::PE_Unknown;
		RelativeTargetDistance = EPlayerDistanceEnum::PE_Unknown;
		return;
	}

	float dist = FVector::Dist(HeadPosition, TargetPosition);

	// Quantize the distance
	if (dist < ShortRange ) {
		RelativeTargetDistance = EPlayerDistanceEnum::PE_Near;
	}
	else if (dist < MediumRange ) {
		RelativeTargetDistance = EPlayerDistanceEnum::PE_Medium;
	}
	else {
		RelativeTargetDistance = EPlayerDistanceEnum::PE_Far;
	}

	// Quantize the position
	RelativeTargetPosition = QuantizePosition( HeadPosition, TargetPosition, CharacterRotation );

	// Convert the boolean values in to an action enum variable
	if (TargetAttacking ) {
		TargetAction = EPlayerActionEnum::PE_Attack;
		return;
	}
	else if (TargetBlocking) {
		TargetAction = EPlayerActionEnum::PE_Block;
		return;
	}
	else if (TargetDodging) {
		TargetAction = EPlayerActionEnum::PE_Dodge;
		return;
	}
	else if (TargetWalking) {
		TargetAction = EPlayerActionEnum::PE_Walk;
		return;
	}

	TargetAction = EPlayerActionEnum::PE_Run;
}

EPlayerPositionEnum UHumanoidSensorsComponent::QuantizePosition(const FVector& HeadPosition, const FVector& TargetPosition, const FRotator& CharacterRotation )
{
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

	// Body rotation

	float Angle = CharacterRotation.Yaw - 90;
	if (Angle > 360)
	{
		Angle -= 360;
	}
	else if (Angle < 0)
	{
		Angle += 360;
	}

	if ((Angle > TargetYaw + FrontAngle ) || (Angle < 45 && TargetYaw > 315))
	{
		return EPlayerPositionEnum::PE_Right;
	}
	else if ((Angle < TargetYaw - FrontAngle) || (TargetYaw < 45 && Angle > 315))
	{
		return EPlayerPositionEnum::PE_Left;
	}

	return EPlayerPositionEnum::PE_Front;
}

