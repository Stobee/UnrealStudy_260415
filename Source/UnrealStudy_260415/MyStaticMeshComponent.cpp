// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStaticMeshComponent.h"

UMyStaticMeshComponent::UMyStaticMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> P38_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));

	if (P38_Propeller.Succeeded())
	{
		SetStaticMesh(P38_Propeller.Object);
	}
}

void UMyStaticMeshComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	AddRelativeRotation(FRotator(0, 0, RotationSpeed * DeltaTime));
}
