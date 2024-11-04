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
	
	// BEGIN - Flashlight components construction
	UFUNCTION(BlueprintCallable)
	bool ConstructFlashlight();
	bool AddSpringArm();
	bool AddSpotLight();
	// END - Flashlight components construction
	
	// BEGIN - Flashlight configuration
	UFUNCTION(BlueprintCallable)
	void SetFlashlightProperties(
		UMaterialInstance* InMaterial,
		bool bInStartOn,
		bool bInBlock,
		float InLagSpeed,
		FVector InLocation,
		float InIntensity);
	void ConfigureFlashlight();
	// END - Flashlight configuration
	
	// BEGIN - Flashlight functionality
	UFUNCTION(BlueprintCallable)
	void ToggleFlashlight();
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

	// Components
	UPROPERTY(EditAnywhere, Category = "Flashlight")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Flashlight")
	USpotLightComponent* SpotLight;

	// Internal flashlight state
	bool bFlashLightOn = true;
};
