// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleInteractable.h"
#include "Interactable.h"
#include "SimpleGrappleInteractTrigger.generated.h"



UCLASS()
class METEORSHOWER_GP3_API ASimpleGrappleInteractTrigger : public AActor, public IGrappleInteractable
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Interactable")
	AActor* InteractableObject;
	ASimpleGrappleInteractTrigger();
	UFUNCTION(BlueprintImplementableEvent,  Category = "Interact")
	void OnInteract();
protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
public:
	virtual void Interact() override;
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	
};
