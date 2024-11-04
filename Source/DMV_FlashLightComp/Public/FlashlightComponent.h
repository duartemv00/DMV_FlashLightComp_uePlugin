// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FlashlightComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DMV_FLASHLIGHTCOMP_API UFlashlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFlashlightComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool bDebug = false;
	
	// BEGIN - Flashlight components construction
	UFUNCTION(BlueprintCallable)
	bool ConstructFlashlight();
	bool AddSpringArm();
	bool AddSpotLight();
	// END - Flashlight components construction
	
	// BEGIN - Flashlight configuration
	UFUNCTION(BlueprintCallable)
	void SetFlashlightProperties(
		bool bInDebug,
		UMaterialInstance* InMaterial,
		bool bInStartOn,
		bool bInBlock,
		float InLagSpeed,
		FVector InLocation,
		float InIntensity,
		bool bInUseBattery,
		float InBatteryLife,
		float InBatteryDrainRatePerSecond,
		float InBatteryRechargeRatePerSecond);

	void ConfigureFlashlight();
	// END - Flashlight configuration
	
	// BEGIN - Flashlight functionality
	UFUNCTION(BlueprintCallable)
	void ToggleFlashlight();
	UFUNCTION(BlueprintCallable)
	void FlashlightOn();
	UFUNCTION(BlueprintCallable)
	void FlashlightOff();
	// END - Flashlight functionality

private:
	// Flashlight properties

	UPROPERTY(EditAnywhere, Category = "Flashlight")
	UMaterialInstance* FlashlightMaskMaterial;

	UPROPERTY(EditAnywhere, Category = "Flashlight")
	float FlashlightLagSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Flashlight")
	float FlashlightIntensity = 100000.0f;

	UPROPERTY(EditAnywhere, Category = "Flashlight")
	FVector FlashlightLocation = FVector(0.0f, 20.0f, 40.0f);

	UPROPERTY(EditAnywhere, Category = "Flashlight")
	bool bBlockFlashlight = false;

	// Batery properties
	UPROPERTY(EditAnywhere, Category = "Flashlight|Battery")
	bool bUseBattery = false;
	
	UPROPERTY(EditAnywhere, Category = "Flashlight|Battery")
	float BatteryLife = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Flashlight|Battery")
	float BatteryDrainRatePerSecond = 0.01f;

	UPROPERTY(EditAnywhere, Category = "Flashlight|Battery")
	float BatteryRechargeRatePerSecond = 0.01f;

	FTimerHandle BatteryTimerHandle;
	void Start_ReduceBatteryTimer();
	void Stop_ReduceBatteryTimer();
	void ReduceBatteryTimer();

	// Components
	UPROPERTY()
	USpringArmComponent* SpringArm;

	UPROPERTY()
	USpotLightComponent* SpotLight;

	// Internal flashlight state
	UPROPERTY()
	bool bFlashLightOn = true;
};
