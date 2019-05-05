// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "ProjectileBall.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "Sockets.h"
#include "GameEventProducer.h"
#include "InGamePlatePawn.generated.h"

UCLASS()
class ARCANOIDCLIENT_API AInGamePlatePawn : public APawn
{
	GENERATED_BODY()
private:
	UCameraComponent* OurCamera;
	GameEventProducer* EventProducer;
public:
	// Sets default values for this pawn's properties
	AInGamePlatePawn();
	virtual void OnConstruction(const FTransform & Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* VisibleComponent;

	// Gun muzzle's offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector CameraLocation;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectileBall> ProjectileClass;

	//Input functions
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void StartGrowing();
	void StopGrowing();
	void Fire();

	void connectServer();

	//Input variables
	FVector CurrentVelocity;
	bool bGrowing;

};
