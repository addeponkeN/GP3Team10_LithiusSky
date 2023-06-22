// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrapplingHookObject.generated.h"

UCLASS()
class METEORSHOWER_GP3_API AGrapplingHookObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapplingHookObject();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Hook)
	UStaticMeshComponent* StaticMesh;

	UStaticMeshComponent*GetStaticMesh();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
