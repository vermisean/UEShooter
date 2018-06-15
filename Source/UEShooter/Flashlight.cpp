// Copyright Sean Duffy 2018

#include "Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AFlashlight::AFlashlight(const FObjectInitializer& ObjectInitializer)
{
	LightAttachPoint = TEXT("LightSocket");

	
	LightConeComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightConeComp"));
	LightConeComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	LightConeComp->AttachTo(RootComponent, LightAttachPoint, EAttachLocation::SnapToTarget);

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComp"));
	SpotLightComp->AttachTo(RootComponent, LightAttachPoint, EAttachLocation::SnapToTarget);
	SpotLightComp->AddLocalRotation(FRotator(0, -90, 0));

	LastEmissiveStrength = -1.0f;

	EmissiveParamName = TEXT("Brightness");
	MaxEmissiveIntensity = 5.0f;
}

void AFlashlight::UpdateLight(bool Enabled)
{
	/* Turn off light while  */
	SpotLightComp->SetVisibility(Enabled);
	LightConeComp->SetVisibility(Enabled);

	/* Update material parameter */
	if (MatDynamic)
	{
		/* " Enabled ? MaxEmissiveIntensity : 0.0f " picks between first or second value based on "Enabled" boolean */
		MatDynamic->SetScalarParameterValue(EmissiveParamName, Enabled ? MaxEmissiveIntensity : 0.0f);
	}
}

// Called when the game starts or when spawned
void AFlashlight::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateLight(bIsActive);
}

