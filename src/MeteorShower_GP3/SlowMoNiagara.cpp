#include "SlowMoNiagara.h"

#include "MeteorShower_GP3.h"

USlowMoNiagara::USlowMoNiagara()
{
}

void USlowMoNiagara::BeginPlay()
{
	Super::BeginPlay();

	auto ParentMesh = Cast<UStaticMeshComponent>(GetAttachParent());

	if(!ParentMesh)
	{
		LOGW("SlowMoNiagara parent was not a StaticMeshComponent")
		DestroyComponent();
		return;
	}
	
	const wchar_t* AssetName = TEXT("/Game/TestFolders/Andreas/FX/NS_TimeStopped.NS_TimeStopped");
	UNiagaraSystem* SlowMoNiagaraSystem = Cast<UNiagaraSystem>(
		StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, AssetName));
	if (SlowMoNiagaraSystem)
	{
		SetAsset(SlowMoNiagaraSystem);
		LOG("Set NiagaraAsset");
	}
	else
	{
		LOGEF("Failed to load SlowMoNiagaraSystem: '%s'", AssetName);
		DestroyComponent();
		return;
	}

	const FString Name = "ParticleMesh";
	SetNiagaraVariableObject(Name, ParentMesh);

	LOGI("Set SlowMo mesh binding");
}
