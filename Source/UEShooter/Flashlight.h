// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flashlight.generated.h"

UCLASS()
class UESHOOTER_API AFlashlight : public AActor
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

private:

	/* Material instance to manipulate emissive on the flashlight instance  */
	class UMaterialInstanceDynamic* MatDynamic;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName EmissiveParamName;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	float MaxEmissiveIntensity;

	/* Last frame's emissive intensity for performance purposes */
	float LastEmissiveStrength;

public:

	AFlashlight(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "Flashlight")
	bool bIsActive;

	UPROPERTY(EditDefaultsOnly)
	FName LightAttachPoint;

	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* LightConeComp;

	UPROPERTY(VisibleDefaultsOnly)
	class USpotLightComponent* SpotLightComp;

	void UpdateLight(bool Enabled);
};
