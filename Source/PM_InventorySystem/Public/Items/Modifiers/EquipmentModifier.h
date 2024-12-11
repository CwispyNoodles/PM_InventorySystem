// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMAbilitySystemComponent.h"
#include "Items/Modifiers/ItemModifier.h"
#include "EquipmentModifier.generated.h"

class UAttributeSet;
/**
 * 
 */
UCLASS(Abstract)
class PM_INVENTORYSYSTEM_API UEquipmentModifier : public UItemModifier
{
	GENERATED_BODY()

public:
	// Used for things like weapons, consumables, etc. Basically for actors that appear in the character's hand or body.
	// If EquipmentItemActor is already instantiated, it will simply return that instead.
	UFUNCTION(BlueprintCallable)
	AItemActor* SpawnEquipmentActor(FTransform SpawnTransform);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AItemActor* GetEquipmentItemActor() { return EquipmentItemActor; }
	
	virtual void Tick(float InDeltaTime) PURE_VIRTUAL("Tick");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	TArray<TObjectPtr<UAbilitySet>> AbilitySetsToGrant;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	TArray<TObjectPtr<UAttributeSet>> AttributeSetsToGrant;

private:
	UPROPERTY()
	AItemActor* EquipmentItemActor;
};
