// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryHUDWidget.h"

#include "InventorySubsystem.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Items/ItemInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/ItemWidget.h"

void UInventoryHUDWidget::AddChildWidget(UUserWidget* NewChildWidget)
{
	UCanvasPanelSlot* CPS = HUDCanvas->AddChildToCanvas(NewChildWidget);
	CPS->SetAutoSize(true);
	CPS->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	CPS->SetOffsets(FMargin(0.0f));
}

FReply UInventoryHUDWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply =  Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	UInventorySubsystem* InventorySubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInventorySubsystem>();
	if (InventorySubsystem->IsDraggingItem() && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UItemInstance* DraggedItem = InventorySubsystem->GetDraggedItem();
		DraggedItem->SetOwner(nullptr);
		DraggedItem->SpawnItemActor(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorTransform());
		InventorySubsystem->ClearItemDragWidget();
	}
	return Reply;
}
