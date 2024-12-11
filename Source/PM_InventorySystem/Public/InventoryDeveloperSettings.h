// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InventoryDeveloperSettings.generated.h"

class UItemDragWidget;
class UItemWidget;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Inventory Settings"))
class PM_INVENTORYSYSTEM_API UInventoryDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Config, EditAnywhere, Category="User Interface")
	TSubclassOf<UItemWidget> DefaultItemWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Config, EditAnywhere, Category="User Interface")
	TSubclassOf<UItemDragWidget> DefaultItemDragWidgetClass;

	UPROPERTY(BlueprintReadOnly, Config, EditAnywhere, Category="User Interface", meta=(ClampMin="1.0"))
	float TileSize = 50.0f;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="ItemContainers")
	FColor ValidTileColor = FColor::Green.WithAlpha(255.0f/2.0f);

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="ItemContainers")
	FColor InvalidTileColor = FColor::Red.WithAlpha(255.0f/2.0f);

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="ItemContainers")
	FColor ObstructionTileColor = FColor::Yellow.WithAlpha(255.0f/2.0f);

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="ItemContainers")
	FColor DefaultItemWidgetBackgroundColor = FColor::Black.WithAlpha(255.0f/4.0f);
};
