// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryHUDWidget.generated.h"

class UCanvasPanel;
class UBorder;
/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UInventoryHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddChildWidget(UUserWidget* NewChildWidget);
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UCanvasPanel> HUDCanvas;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UBorder> Background;


};
