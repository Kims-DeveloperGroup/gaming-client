// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Engine/Private/InstancedStaticMesh.h"
#include "Runtime/Engine/Private/InstancedStaticMesh.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"

#include "PropGeneratedActor.generated.h"

UCLASS()
class ARCANOIDCLIENT_API APropGeneratedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APropGeneratedActor();
	virtual void OnConstruction(const FTransform & Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector2D ScaleXY;

	UPROPERTY(EditAnywhere)
		USceneComponent* VisibleComponent;

	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* InstancedStaticMeshComp;

	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;
	
	UPROPERTY(EditAnywhere, Category = "MeshSize")
		FVector2D MeshSize;


private:

	FVector2D CurrentScale;
};
