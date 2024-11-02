// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	UFUNCTION()
	bool AddSpringArm();
	UFUNCTION()
	bool AddSpotLight();
	// END - Flashlight components construction

	// BEGIN - Flashlight functionality
	UFUNCTION(BlueprintCallable)
	void ToggleFlashlight();
	// END - Flashlight functionality

private:
	UPROPERTY(EditAnywhere)
	UMaterialInstance* MaskMaterial;
	
	UPROPERTY()
	UActorComponent* SpringArm;
	UPROPERTY()
	UActorComponent* SpotLight;
	UPROPERTY()
	
	bool bFlashLightOn = false;
};
