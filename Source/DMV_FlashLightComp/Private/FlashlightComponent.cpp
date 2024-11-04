// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightComponent.h"

#include "Components/SpotLightComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"


UFlashlightComponent::UFlashlightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructFlashlight();
	ConfigureFlashlight();
}

bool UFlashlightComponent::ConstructFlashlight()
{
	if(AddSpringArm()) if(AddSpotLight()) return true;
	return false;
}

bool UFlashlightComponent::AddSpringArm()
{
	UActorComponent* SpringArmCreation = GetOwner()->AddComponentByClass(
		USpringArmComponent::StaticClass(), true, FTransform(), true);
	if (SpringArmCreation) SpringArmCreation->RegisterComponent();
	else UE_LOG(LogTemp, Error, TEXT("SpringArm not created"));
	
	SpringArm = Cast<USpringArmComponent>(SpringArmCreation);

	// Attach SpringArm to Owner
	SpringArm->SetWorldLocation(GetOwner()->GetActorLocation());
	SpringArm->AttachToComponent(
		Cast<USceneComponent>(GetOwner()->GetRootComponent()), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraRotationLag = true;
	
	return true;
}

bool UFlashlightComponent::AddSpotLight()
{
	UActorComponent* SpotLightCreation = GetOwner()->AddComponentByClass(
		USpotLightComponent::StaticClass(), true, FTransform(), true);
	if (SpotLightCreation) SpotLightCreation->RegisterComponent();
	else UE_LOG(LogTemp, Error, TEXT("SpotLight not created"));
	
	SpotLight = Cast<USpotLightComponent>(SpotLightCreation);
	
	// Attach SpotLight to SpringArm
	SpotLight->SetWorldLocation(GetOwner()->GetActorLocation());
	SpotLight->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	
	return true;
}

void UFlashlightComponent::SetFlashlightProperties(
	UMaterialInstance* InMaterial,
		bool bInStartOn,
		bool bInBlock,
		float InLagSpeed,
		FVector InLocation,
		float InIntensity)
{
	FlashlightLocation = InLocation;
	FlashlightLagSpeed = InLagSpeed;
	FlashlightIntensity = InIntensity;
	FlashlightMaskMaterial = InMaterial;
	bBlockFlashlight = bInBlock;
	bFlashLightOn = bInStartOn;
}

void UFlashlightComponent::ConfigureFlashlight()
{
	SpringArm->SetWorldLocation(FlashlightLocation);
	SpringArm->CameraRotationLagSpeed = FlashlightLagSpeed;
	SpotLight->SetIntensity(FlashlightIntensity);
	if(FlashlightMaskMaterial) SpotLight->SetLightFunctionMaterial(FlashlightMaskMaterial);
	if(bFlashLightOn) SpringArm->SetVisibility(true, true);
	else SpringArm->SetVisibility(false, true);
}

void UFlashlightComponent::ToggleFlashlight()
{
	if(!bBlockFlashlight) // Check if flashlight is blocked
	{
		if (bFlashLightOn) {
			SpringArm->SetVisibility(false, true);
			bFlashLightOn = false;
		} else {
			SpringArm->SetVisibility(true, true);
			bFlashLightOn = true;
		}
	}
}
