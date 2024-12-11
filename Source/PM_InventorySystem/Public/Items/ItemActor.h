// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS(Abstract, NotBlueprintable)
class PM_INVENTORYSYSTEM_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

UCLASS(Blueprintable)
class PM_INVENTORYSYSTEM_API ASkeletalMeshItemActor : public AItemActor
{
	GENERATED_BODY()

public:
	ASkeletalMeshItemActor();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USkeletalMeshComponent* ItemSkeletalMesh;
};

UCLASS(Blueprintable)
class PM_INVENTORYSYSTEM_API AStaticMeshItemActor : public AItemActor
{
	GENERATED_BODY()

};
