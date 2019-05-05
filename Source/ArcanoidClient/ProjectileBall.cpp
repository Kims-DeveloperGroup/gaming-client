// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBall.h"

// Sets default values
AProjectileBall::AProjectileBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjecttileMovement"));
	ProjectileMovement->SetUpdatedComponent(CollisionComponent);
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 6000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 1.0f;
}

// Called when the game starts or when spawned
void AProjectileBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBall::FireInDirection(const FVector & ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

