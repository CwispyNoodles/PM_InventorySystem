// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryVerbMessage.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "ItemWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UBorder;
class UImage;
class USizeBox;
class UItemInstance;
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_UI_ItemWidget_Dragged);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_UI_ItemWidget_Dropped);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemWidgetHandledEvent, UItemInstance*, ItemInstance);

UCLASS()
class PM_INVENTORYSYSTEM_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	friend UItemInstance;

	// UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeDestruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	// UUserWidget Interface End

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UItemInstance* GetItemInstance() { return ItemInstance; }

	void InitializeItemWidget(UItemInstance* InItemInstance);

	UPROPERTY(BlueprintAssignable)
	FItemWidgetHandledEvent OnItemRemoved;

	UPROPERTY(BlueprintAssignable)
	FItemWidgetHandledEvent OnItemAdded;

	UFUNCTION(BlueprintImplementableEvent)
	void Refresh();

	UFUNCTION()
	void HandleIsDirty();
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TSoftObjectPtr<APCIconCreator> IconCreatorRef;
 
private:
	UFUNCTION()
	void OnItemWidgetDragged(FGameplayTag InChannel, const FInventoryVerbMessage& InMessage);
	
	UFUNCTION()
	void OnItemWidgetDropped(FGameplayTag InChannel, const FInventoryVerbMessage& InMessage);
	
	UFUNCTION(BlueprintCallable)
	void SetItemWidgetBounds(FVector2D InBounds) const;
	
	UFUNCTION(BlueprintCallable)
	void HandleIsDraggingItemWidget();

	UFUNCTION(BlueprintCallable)
	void HandleIsItemRotated();
	
	UPROPERTY()
	TObjectPtr<UItemInstance> ItemInstance;

	FGameplayMessageListenerHandle ItemWidgetDraggedListenerHandle;
	FGameplayMessageListenerHandle ItemWidgetDroppedListenerHandle;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool bIsDirty = true;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<UTexture2D> CachedItemIcon;


protected:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<USizeBox> Bounds;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UBorder> QualityIndicator;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UProgressBar> DurabilityBar;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextBlock> QuantityText;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextBlock> AmmoText;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UBorder> DropIndicator;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UBorder> Background;
};
