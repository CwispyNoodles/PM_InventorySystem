// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDisplayWidget.generated.h"

class UImage;
class USizeBox;
class UItemInstance;
/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UItemDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeItemDisplayWidget(UItemInstance* InItemInstance);

private:
	UPROPERTY()
	TObjectPtr<UItemInstance> ItemInstance;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<USizeBox> Bounds;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UImage> ItemIcon;
};
