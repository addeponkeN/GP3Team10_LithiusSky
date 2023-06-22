// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappableOverlay.h"

// Sets default values for this component's properties
UGrappableOverlay::UGrappableOverlay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
void UGrappableOverlay::BeginPlay()
{
	GetOwner()->GetComponents<UStaticMeshComponent>(MeshComponents);
}


void UGrappableOverlay::OnLookBegin()
{
	if (!MeshComponents.IsEmpty())
	{
		for(UStaticMeshComponent* Mesh: MeshComponents)
		{
			Mesh->SetRenderCustomDepth(true);	
		}
	}
	OnLookBegin_Implementable.Broadcast();
}
void UGrappableOverlay::OnLookEnd()
{
	if (!MeshComponents.IsEmpty())
	{
		for(UStaticMeshComponent* Mesh: MeshComponents)
		{
			Mesh->SetRenderCustomDepth(false);	
		}
	}
	OnLookEnd_Implementable.Broadcast();
}




