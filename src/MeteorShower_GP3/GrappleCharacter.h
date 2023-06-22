// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeteorShower_GP3Character.h"
#include "GrappleCharacter.generated.h"

UCLASS()
class METEORSHOWER_GP3_API AGrappleCharacter : public AMeteorShower_GP3Character
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGrappleCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
