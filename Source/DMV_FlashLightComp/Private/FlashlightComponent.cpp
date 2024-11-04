// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightComponent.h"

#include "Components/SpotLightComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

/**
 * Constructor
 */
UFlashlightComponent::UFlashlightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

/**
 * Called when the game starts
 */
void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructFlashlight();
	ConfigureFlashlight();
}

/**
 * Construct the flashlight components
 * @return Whether the flashlight components were constructed successfully
 */
bool UFlashlightComponent::ConstructFlashlight()
{
	if(AddSpringArm()) if(AddSpotLight()) return true;
	return false;
}

/**
 * Add a SpringArm component to the owner
 * @return Whether the SpringArm component was added successfully
 */
bool UFlashlightComponent::AddSpringArm()
{
	UActorComponent* SpringArmCreation = GetOwner()->AddComponentByClass(
		USpringArmComponent::StaticClass(), true, FTransform(), true);
	if (SpringArmCreation) SpringArmCreation->RegisterComponent();
	else UE_LOG(LogTemp, Error, TEXT("SpringArm not created"));
	
	SpringArm = Cast<USpringArmComponent>(SpringArmCreation);

	// Attach SpringArm to Owner
	SpringArm->AttachToComponent(
		Cast<USceneComponent>(GetOwner()->GetRootComponent()), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraRotationLag = true;
	
	return true;
}

/**
 * Add a SpotLight component to the owner
 * @return Whether the SpotLight component was added successfully
 */
bool UFlashlightComponent::AddSpotLight()
{
	UActorComponent* SpotLightCreation = GetOwner()->AddComponentByClass(
		USpotLightComponent::StaticClass(), true, FTransform(), true);
	if (SpotLightCreation) SpotLightCreation->RegisterComponent();
	else UE_LOG(LogTemp, Error, TEXT("SpotLight not created"));
	
	SpotLight = Cast<USpotLightComponent>(SpotLightCreation);
	
	// Attach SpotLight to SpringArm
	SpotLight->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	
	return true;
}

/**
 * Set the flashlight properties
 * @param bInDebug Activate debug messages and visualization
 * @param InMaterial Material to mask the flashlight
 * @param bInStartOn Whether the flashlight should start on 
 * @param bInBlock Whether the flashlight should be blocked, so you can't turn it on or off 
 * @param InLagSpeed The speed of the flashlight rotation to catch up with the camera rotation
 * @param InLocation The relative location of the flashlight to the owner
 * @param InIntensity The intensity of the flashlight light
 * @param bInUseBattery Whether the flashlight should use battery
 * @param InBatteryLife The total battery life of the flashlight
 * @param InBatteryDrainRatePerSecond The rate at which the battery drains per second
 * @param InBatteryRechargeRatePerSecond The rate at which the battery recharges per second
 */
void UFlashlightComponent::SetFlashlightProperties(
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
		float InBatteryRechargeRatePerSecond)
{
	bDebug = bInDebug;
	FlashlightLocation = InLocation;
	FlashlightLagSpeed = InLagSpeed;
	FlashlightIntensity = InIntensity;
	FlashlightMaskMaterial = InMaterial;
	bBlockFlashlight = bInBlock;
	bFlashLightOn = bInStartOn;
	bUseBattery = bInUseBattery;
	BatteryLife = InBatteryLife;
	BatteryDrainRatePerSecond = InBatteryDrainRatePerSecond;
	BatteryRechargeRatePerSecond = InBatteryRechargeRatePerSecond;

	ConfigureFlashlight();
}

/**
 * Configure the flashlight with the setted properties
 */
void UFlashlightComponent::ConfigureFlashlight()
{
	SpringArm->SetRelativeLocation(FlashlightLocation);
	SpringArm->CameraRotationLagSpeed = FlashlightLagSpeed;
	SpotLight->SetIntensity(FlashlightIntensity);
	if(FlashlightMaskMaterial) SpotLight->SetLightFunctionMaterial(FlashlightMaskMaterial);
	if(bFlashLightOn) FlashlightOn();
	else FlashlightOff();
}

/**
 * Toggle the flashlight on and off
 */
void UFlashlightComponent::ToggleFlashlight()
{
	if (bFlashLightOn) {
		FlashlightOff();
	} else {
		FlashlightOn();
	}
}

/**
 * Turn on the flashlight
 */
void UFlashlightComponent::FlashlightOn()
{
	if(!bBlockFlashlight) // Check if flashlight is blocked
	{
		SpringArm->SetVisibility(true, true);
		bFlashLightOn = true;
		if(bUseBattery) Start_ReduceBatteryTimer();
	}
}

/**
 * Turn off the flashlight
 */
void UFlashlightComponent::FlashlightOff()
{
	if(!bBlockFlashlight) // Check if flashlight is blocked
	{
		SpringArm->SetVisibility(false, true);
		bFlashLightOn = false;
		if(bUseBattery) Stop_ReduceBatteryTimer();
	}
}

/**
 * Call the timer function to reduce the battery life
 */
void UFlashlightComponent::Start_ReduceBatteryTimer()
{
	GetWorld()->GetTimerManager().SetTimer(BatteryTimerHandle, this,
		&UFlashlightComponent::ReduceBatteryTimer, 1.f, true, 0.f);
}

/**
 * Stop the timer function to reduce the battery life
 */
void UFlashlightComponent::Stop_ReduceBatteryTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(BatteryTimerHandle);
}

/**
 * Timer function that reduces the battery life
 */
void UFlashlightComponent::ReduceBatteryTimer()
{
	if(BatteryLife > 0.0f)
	{
		BatteryLife -= BatteryDrainRatePerSecond;
		if (bDebug) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Battery: %f"), BatteryLife));
	}
	else FlashlightOff();
}
