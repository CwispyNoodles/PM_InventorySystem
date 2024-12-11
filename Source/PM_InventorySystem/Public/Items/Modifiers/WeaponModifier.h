// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Items/Modifiers/EquipmentModifier.h"
#include "WeaponModifier.generated.h"

USTRUCT(BlueprintType)
struct FWeaponAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<UAnimMontage> EquipAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<UAnimMontage> UnequipAnimation;
};
	
/**
 * 
 */
UCLASS(Abstract)
class PM_INVENTORYSYSTEM_API UWeaponModifier : public UEquipmentModifier
{
	GENERATED_BODY()

public:
	// The animation layer to switch to when equipping this item.
	// When you unequip this item, switch to the DefaultAnimationLayer set in the EquipmentManagerComponent.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TSubclassOf<UAnimInstance> EquipmentAnimationLayer;    

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TMap<FGameplayTag, FWeaponAnimations> CharacterEquipAnimations;
};
