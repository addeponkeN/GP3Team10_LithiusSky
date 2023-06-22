// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleGrappleInteractTrigger.h"

// Sets default values
ASimpleGrappleInteractTrigger::ASimpleGrappleInteractTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}
void ASimpleGrappleInteractTrigger::Interact()
{
	OnInteract();
	if(IInteractable* CastInteracableObject = Cast<IInteractable>(InteractableObject))
	{
		CastInteracableObject->Interact();
	}
}



