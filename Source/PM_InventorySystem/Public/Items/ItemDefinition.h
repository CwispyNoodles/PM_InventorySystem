// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Modifiers/ItemModifier.h"
#include "ItemDefinition.generated.h"

class AItemActor;
/**
 * 
 */
UCLASS(Const)
class PM_INVENTORYSYSTEM_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	FGameplayTag ItemTag;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	// TObjectPtr<UPCContextMenuActionSet> ItemContextMenuActionSet;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	// TSubclassOf<UPCContextMenuAction> DefaultActionClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flavor")
	FText DisplayName = FText::FromString("PLACEHOLDER");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flavor", meta=(MultiLine=true))
	FText Description = FText::FromString("PLACEHOLDER");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="User Interface", meta=(ClampMin="1"))
	FIntPoint Dimensions = FIntPoint(2);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="User Interface", meta=(InlineEditConditionToggle))
	bool bThumbnailManualOverride = false;

	// Enable when you want to override automatic Icon creation.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="User Interface", meta=(EditCondition="bThumbnailManualOverride==true"))
	UTexture2D* Thumbnail;

	// The sound that plays when the player begins to pick-up the Item Widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="User Interface|Sounds")
	TObjectPtr<USoundBase> WidgetOnPickUpSound;

	// The sound that plays when the player drops the item widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="User Interface|Sounds")
	TObjectPtr<USoundBase> WidgetOnDropSound;

	// The sound that plays when the player adds to the stack.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="User Interface|Sounds")
	TObjectPtr<USoundBase> WidgetOnStackAddedSound;

	// The sound that plays when the player subtracts to the stack.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="User Interface|Sounds")
	TObjectPtr<USoundBase> WidgetOnStackSubtractedSound;

	// The Actor class used to represent the item while spawned in the world.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Scene")
	TSubclassOf<AItemActor> WorldActorClass;

	// The Actor class used to represent the item while inside of a container. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Scene")
	TSubclassOf<AItemActor> InventoryActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Statistics", meta=(ClampMin="1"))
	int32 MaxStackAmount = 1;

	// Declare an attribute with an assigned tag and default value.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Statistics", meta=(ClampMin="0.0", Delta="0.01"))
	TMap<FGameplayTag, float> ItemAttributes;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Statistics", meta=(InlineEditConditionToggle))
	// bool bHasDurability = false;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Statistics", meta=(EditCondition="bHasDurability"))
	// int32 DurabilityPoints = 1;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Module")
	// TArray<TSubclassOf<UPCItemModule>> Modules;
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category="Modifiers")
	TArray<UItemModifier*> Modifiers;
};
