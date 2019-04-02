// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HumanoidSensorsComponent.generated.h"

// Changes in the numbers of the enum values must be updated to the LookUpTable size for the boss agent!

UENUM(BlueprintType)
enum class EPlayerActionEnum : uint8
{
	PE_Walk		UMETA(DisplayName = "Walk"),
	PE_Run		UMETA(DisplayName = "Run"),
	PE_Attack	UMETA(DisplayName = "Attack"),
	PE_Block	UMETA(DisplayName = "Block"),
	PE_Dodge	UMETA(DisplayName = "Dodge"),
	PE_Dead		UMETA(DisplayName = "Dead"),
	PE_Unknown	UMETA(DisplayName = "Unknown")
};

UENUM(BlueprintType)
enum class EPlayerPositionEnum : uint8
{
	PE_Front		UMETA(DisplayName = "Front"),
	PE_Left			UMETA(DisplayName = "Left"),
	PE_Right		UMETA(DisplayName = "Right"),
	PE_Unknown		UMETA(DisplayName = "Unknown")
};

UENUM(BlueprintType)
enum class EPlayerDistanceEnum : uint8
{
	PE_Near			UMETA(DisplayName = "Near"),
	PE_Medium		UMETA(DisplayName = "Medium"),
	PE_Far			UMETA(DisplayName = "Far"),
	PE_Unknown		UMETA(DisplayName = "Unknown")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSFIGHT_API UHumanoidSensorsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHumanoidSensorsComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightConeAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FrontAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShortRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MediumRange;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	float SightConeAngleCosine;
	EPlayerPositionEnum QuantizePosition(const FVector& HeadPosition, const FVector& TargetPosition, const FRotator& CharacterRotation);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PostInitProperties() override;

	UFUNCTION(BlueprintCallable, Category = "Artificial intelligence")
		void ProducePercepts(const FVector& TargetPosition, const FVector& HeadPosition, const FRotator& CharacterRotation, const FVector& GazeDirection,
			const bool& TargetWalking, const bool& TargetAttacking, const bool& TargetBlocking, const bool& TargetDodging,
			EPlayerActionEnum& TargetAction, EPlayerPositionEnum& RelativeTargetPosition, EPlayerDistanceEnum& RelativeTargetDistance );
};
