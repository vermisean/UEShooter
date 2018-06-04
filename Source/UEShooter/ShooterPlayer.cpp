// Copyright Sean Duffy 2018

#include "ShooterPlayer.h"
#include "Camera/CameraComponent.h"

// Sets default values
AShooterPlayer::AShooterPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create FPS camera, attach to capsule, and set up position
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	FPSCameraComponent->SetupAttachment(GetRootComponent());
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create and set up the skeletal mesh
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS Mesh"));
	FPSMesh->SetOnlyOwnerSee(true);
	FPSMesh->SetupAttachment(FPSCameraComponent);
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// Disable ThirdPerson mesh for player
	GetMesh()->SetOwnerNoSee(true);

}

// Called when the game starts or when spawned
void AShooterPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShooterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up movement bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterPlayer::MoveRight);

	// Set up look bindings
	PlayerInputComponent->BindAxis("Turn", this, &AShooterPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterPlayer::AddControllerPitchInput);

	// Set up jump bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterPlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AShooterPlayer::StopJump);

	// Set up firing bindings
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterPlayer::Fire);
}

// Movement
void AShooterPlayer::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AShooterPlayer::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

// Jumping
void AShooterPlayer::StartJump()
{
	bPressedJump = true;
}

void AShooterPlayer::StopJump()
{
	bPressedJump = false;
}

void AShooterPlayer::Fire()
{

}

