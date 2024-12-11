// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDragWidget.generated.h"

class UItemInstance;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UItemDragWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeItemDragWidget(UItemInstance* ItemToDrag);

	UFUNCTION(BlueprintCallable)
	void UpdatePosition();
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCanvasPanel> MouseBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UNamedSlot> NamedSlot;

private:
	// UFUNCTION()
	// void SetItemDragWidgetVisibility();
	//
	// FTimerHandle InitializeHandle;
	protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
