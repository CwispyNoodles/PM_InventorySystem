// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Items/Modifiers/ItemModifier.h"
#include "WeaponAmmunitionModifier.generated.h"


/**
 *
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UWeaponAmmunitionModifier : public UItemModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammunition", meta=(Categories="Item.Ammo"))
	FGameplayTag AmmoTag;
};
