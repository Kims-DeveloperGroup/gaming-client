// Fill out your copyright notice in the Description page of Project Settings.

#include "InGamePlatePawn.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Networking/Public/Interfaces/IPv4/IPv4Address.h" 
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "ProjectileBall.h"

AInGamePlatePawn::AInGamePlatePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-1000.0f, 0.0f, 0.0f));
	OurCamera->SetRelativeRotation(FRotator(20.0f, 0.0f, 0.0f));

	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	VisibleComponent->SetupAttachment(RootComponent);
}

void AInGamePlatePawn::OnConstruction(const FTransform & Transform)
{
	OurCamera->SetRelativeLocation(CameraLocation);
}

void AInGamePlatePawn::BeginPlay()
{
	Super::BeginPlay();
	this->connectServer();
}

// Called every frame
void AInGamePlatePawn::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);
	{
		float CurrentScale = VisibleComponent->GetComponentScale().X;
		if (bGrowing)
		{
			CurrentScale += DeltaTime;
		}
		else
		{
			CurrentScale -= (DeltaTime * .5f);
		}

		CurrentScale = FMath::Clamp(CurrentScale, 1.f, 2.f);
		VisibleComponent->SetWorldScale3D(FVector(CurrentScale));
	}
	{
		if (!CurrentVelocity.IsZero())
		{
			FVector NewLocacation = GetActorLocation() + (CurrentVelocity * DeltaTime);
			SetActorLocation(NewLocacation);
		}
	}
	if (EventProducer == 0)
	{
		this->connectServer();
	}
	EventProducer->SendEvent(HEALTH);
}

// Called to bind functionality to input
void AInGamePlatePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Respond when our "Grow" key is pressed or released.
	PlayerInputComponent->BindAction("Grow", IE_Pressed, this, &AInGamePlatePawn::StartGrowing);
	PlayerInputComponent->BindAction("Grow", IE_Released, this, &AInGamePlatePawn::StopGrowing);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	PlayerInputComponent->BindAxis("MoveX", this, &AInGamePlatePawn::Move_XAxis);
	PlayerInputComponent->BindAxis("MoveY", this, &AInGamePlatePawn::Move_YAxis);

	//Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AInGamePlatePawn::Fire);

}

void AInGamePlatePawn::Move_XAxis(float AxisValue)
{
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
}

void AInGamePlatePawn::Move_YAxis(float AxisValue)
{
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
}

void AInGamePlatePawn::StartGrowing()
{
	bGrowing = true;
}

void AInGamePlatePawn::StopGrowing()
{
	bGrowing = false;
}

void AInGamePlatePawn::Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fire"));
	if (ProjectileClass)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector MuzzleLocation = CameraLocation + FTransform(CameraLocation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		MuzzleRotation.Pitch += 90.0f;
		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			AProjectileBall* Projectile = World->SpawnActor<AProjectileBall>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}

		}
	}
}

void AInGamePlatePawn::connectServer()
{
	FIPv4Address ip(127, 0, 0, 1);
	int32 port = 9999;
	EventProducer = new GameEventProducer(NULL, port);
	EventProducer->ConnectServer();
}

