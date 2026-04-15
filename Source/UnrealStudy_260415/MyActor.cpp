// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameframework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(25.0f, 20.0f, 20.0f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);
	Body->SetRelativeRotation(FRotator(-90, 0, 0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_Rocket.SM_Rocket'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_Body(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/P38/Materials/MI_Rocket.MI_Rocket'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	if (MI_Body.Succeeded())
	{
		Body->SetMaterial(0, MI_Body.Object);
	}

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 1600.0f;
	Movement->MaxSpeed = 4000.0f;
	Movement->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	FLatentActionInfo F;
	F.CallbackTarget = this;
	F.ExecutionFunction = FName(TEXT("DelayFunction"));
	F.Linkage = 1;
	F.UUID = 0;

	UKismetSystemLibrary::Delay(GetWorld(), 3.0f, F);
	
}

void AMyActor::DelayFunction()
{
	Destroy();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

