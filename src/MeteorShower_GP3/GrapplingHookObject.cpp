// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHookObject.h"

// Sets default values
AGrapplingHookObject::AGrapplingHookObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
}

UStaticMeshComponent* AGrapplingHookObject::GetStaticMesh()
{
	if(!StaticMesh)
	{
		TArray<UStaticMeshComponent*> SMComponents;
		GetComponents<UStaticMeshComponent>(SMComponents);
		StaticMesh = SMComponents[0];
	}
	return StaticMesh;
}

// Called when the game starts or when spawned
void AGrapplingHookObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrapplingHookObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

