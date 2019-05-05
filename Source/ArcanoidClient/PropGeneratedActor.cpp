// Fill out your copyright notice in the Description page of Project Settings.

#include "PropGeneratedActor.h"

// Sets default values
APropGeneratedActor::APropGeneratedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	VisibleComponent->SetupAttachment(RootComponent);
	CurrentScale = ScaleXY;
}
void APropGeneratedActor::OnConstruction(const FTransform & Transform)
{
	//FVector CurrentLoc;
	
	if (ScaleXY.X != CurrentScale.X || ScaleXY.Y != CurrentScale.Y)
	{
		this->UnregisterAllComponents();
		CurrentScale = ScaleXY;
		UInstancedStaticMeshComponent* InstancedStaticMesh;

		for (float x = 0.0f; x < ScaleXY.X; x++)
		{
			for (float y = 0.0f; y < ScaleXY.Y; y++)
			{
				InstancedStaticMesh = NewObject<UInstancedStaticMeshComponent>(this);
				InstancedStaticMesh->RegisterComponent();
				InstancedStaticMesh->SetStaticMesh(Mesh);
				InstancedStaticMesh->AttachToComponent(VisibleComponent, FAttachmentTransformRules::KeepRelativeTransform);
				
				FTransform InstanceTransform = FTransform(FVector(MeshSize.X * x, MeshSize.Y * y, 0.0f));
				InstancedStaticMesh->AddInstance(InstanceTransform);
				AddInstanceComponent(InstancedStaticMesh);
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnConstruction"));
		RegisterAllComponents();
	}
}
// Called when the game starts or when spawned
void APropGeneratedActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APropGeneratedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

