// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LookAtComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSFIGHT_API ULookAtComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULookAtComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BodyRotationLimitYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BodyRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HeadRotationLimitRoll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HeadRotationLimitYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HeadRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotationGoalTolerance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NeutralAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FacingSpeed;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FRotator CurrentBodyRotation;
	FRotator CurrentHeadRotation;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void CalculateRotation( FRotator& BodyRotation, FRotator& HeadRotation, FRotator& CharacterRotationDelta, const bool& Facing, const bool& CanSeeTarget, const FVector& TargetPosition, const FVector& HeadPosition, const FRotator& CharacterRotation, const float& DeltaSeconds );
};
