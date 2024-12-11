// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryHUDComponent.generated.h"


class UInventoryHUDWidget;
class UInventoryWidget;
class UItemDragWidget;
class UItemInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PM_INVENTORYSYSTEM_API UInventoryHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryHUDComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool IsDraggingItem() const;

	UFUNCTION(BlueprintCallable)
	UItemInstance* GetDraggedItem() { return DraggedItem; }
	
	UFUNCTION(BlueprintCallable)
	UItemDragWidget* CreateItemDragWidget(UItemInstance* ItemToDrag);

	UFUNCTION(BlueprintCallable)
	void ClearItemDragWidget();

	UFUNCTION(BlueprintCallable)
	UInventoryHUDWidget* OpenInventory(bool& bSuccess);

	UFUNCTION(BlueprintCallable)
	void CloseInventory();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryHUDWidget* GetInventory(bool& bIsValid);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInventoryOpen();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryHUDWidget> InventoryHUDWidgetClass;
private:
	UPROPERTY()
	UItemInstance* DraggedItem;

	UPROPERTY()
	TObjectPtr<UItemDragWidget> ItemDragWidgetInstance;

	UPROPERTY()
	TObjectPtr<UInventoryHUDWidget> InventoryHUDWidgetInstance;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
