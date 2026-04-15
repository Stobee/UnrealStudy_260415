// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "MyPlayerController.h"
#include "MyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyStaticMeshComponent.h"
#include "EnhancedInputComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(30.0f, 32.0f, 32.0f));


	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_Body(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/P38/Materials/MI_P38.MI_P38'"));

	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	
	}

	if (MI_Body.Succeeded())
	{
		Body->SetMaterial(0, MI_Body.Object);
	}

	Left = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);
	Right = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	Left->SetRelativeLocation(FVector(40.000000f, -20.000000f, 0.000000f));
	Right->SetRelativeLocation(FVector(40.000000f, 20.000000f, 0.000000f));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(200.0f, 0, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->SocketOffset = FVector(0, 0, 20.0f);
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagMaxDistance = 60.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 500.0f;
	


}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), BoostValue);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Rotate, ETriggerEvent::Triggered, this, &AMyPawn::Rotate);
		UIC->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AMyPawn::Fire);
		UIC->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &AMyPawn::Boost);
		UIC->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AMyPawn::UnBoost);

	}
}


void AMyPawn::Rotate(const FInputActionValue& Value)
{
	FVector2D Rot = Value.Get<FVector2D>();
	Rot = Rot * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60.0f;

	AddActorLocalRotation(FRotator(Rot.Y, 0, Rot.X));
}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(), Arrow->GetComponentTransform());
}

void AMyPawn::Boost()
{
	BoostValue = 1.0f;
	Left->RotationSpeed = 3000.0f;
	Right->RotationSpeed = 3000.0f;
}

void AMyPawn::UnBoost()
{
	BoostValue = 0.5f;
	Left->RotationSpeed = 1200.0f;
	Right->RotationSpeed = 1200.0f;
}

