// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryWheelWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryData/InventoryWheelData.h"
#include "Widgets/ItemDisplayWidget.h"
#include "InventoryData/CommonInventoryData.h"
#include "Items/ItemInstance.h"
#include "Kismet/KismetMathLibrary.h"


UInventoryData* UInventoryWheelWidget::CreateNewInventoryData(UObject* Payload)
{
	if (InventoryData)
	{
		return Cast<UCommonInventoryData>(InventoryData);
	}

	UInventoryWheelData* NewInventoryWheelData = NewObject<UInventoryWheelData>();
	SetInventoryData(NewInventoryWheelData);
	return NewInventoryWheelData;

}

void UInventoryWheelWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float X, Y;
	GetOwningPlayer()->GetMousePosition(X,Y);
	MousePos = FVector2D(X,Y) / UWidgetLayoutLibrary::GetViewportScale(this);
	Follow(MousePos);
	CalculateB();
	CurrentSearchSectorAngle = AngleBetweenVectors(MousePos - End, FVector2D(0.0f, -1.0f));
	CurrentActiveSectorAngle = GetCurrentSector(CurrentActiveSector);
	
}

void UInventoryWheelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWheelWidget::SetInventoryData(UInventoryData* NewInventoryData)
{
	if (UInventoryWheelData* InventoryWheelData = Cast<UInventoryWheelData>(NewInventoryData))
	{
		Super::SetInventoryData(InventoryWheelData);
		NativeRefresh();
	}
}

bool UInventoryWheelWidget::TryAddItem(UItemInstance* ItemToAdd)
{
	if (GetInventoryWheelData()->TryAddItem(ItemToAdd))
	{
		NativeRefresh();
		return true;
	}
	return false;
}

void UInventoryWheelWidget::TryRemoveItem(UItemInstance* ItemToRemove)
{
	if (GetInventoryWheelData()->TryRemoveItem(ItemToRemove))
	{
		NativeRefresh();
	}
}

void UInventoryWheelWidget::AddItemWidget(UItemInstance* NewItemInstance, FVector2D Pos)
{
	check(ItemDisplayWidgetClass)
	UItemDisplayWidget* NewItemDisplayWidget = CreateWidget<UItemDisplayWidget>(GetWorld(), ItemDisplayWidgetClass);
	NewItemDisplayWidget->InitializeItemDisplayWidget(NewItemInstance);
	UCanvasPanelSlot* CPS = InventoryCanvasPanel->AddChildToCanvas(NewItemDisplayWidget);
	CPS->SetAutoSize(true);
	CPS->SetPosition(Pos);
	CPS->SetAnchors(FAnchors(0.5f, 0.5f));
	CPS->SetAlignment(FVector2D(0.5f));
	ItemToWidgetMap.Add(NewItemInstance, NewItemDisplayWidget);
}

void UInventoryWheelWidget::RemoveItemWidget(UItemInstance* NewItemInstance, FVector2D Pos)
{
	UUserWidget* ItemWidgetToRemove;
	if (ItemToWidgetMap.RemoveAndCopyValue(NewItemInstance, ItemWidgetToRemove))
	{
		ItemWidgetToRemove->RemoveFromParent();
	}
}

void UInventoryWheelWidget::NativeRefresh()
{
	TArray<UItemInstance*> Items = GetInventoryWheelData()->GetItems();

	SectorNum = Items.Num();
	SectorSize = 360.0f / SectorNum;
	CalculateSectorBounds();

	// Clear old Item Display Widgets and then create new ones (With new updated data).
	for (UItemInstance* Item: Items)
	{
		RemoveItemWidget(Item, FVector2D());
	}
	
	for (int i = 0; i < SectorNum; i++)
	{
		FVector Dir = UKismetMathLibrary::RotateAngleAxis(FVector(0.0f, -1.0f, 0.0f), i * SectorSize, FVector(0.0f, 0.0f, 1.0f));
		AddItemWidget(Items[i], FVector2D(Dir.X, Dir.Y) * InventoryWheelRadius);
	}
	
	Refresh();
}

UInventoryWheelData* UInventoryWheelWidget::GetInventoryWheelData() const
{
	return Cast<UInventoryWheelData>(InventoryData);
}

void UInventoryWheelWidget::CalculateSectorBounds()
{
	MinSectorBounds.Empty();
	MaxSectorBounds.Empty();

	for (int i = 0; i < SectorNum; i++)
	{
		float Prod = SectorSize * i;
		float Quot = SectorSize / 2.0f;
		MinSectorBounds.Add(UKismetMathLibrary::ClampAxis(Prod - Quot));
		MaxSectorBounds.Add(UKismetMathLibrary::ClampAxis(Prod + Quot));
	}
}

float UInventoryWheelWidget::AngleBetweenVectors(FVector2D InA, FVector2D InB)
{
	const FVector2D Dif = InB - InA;
	return UKismetMathLibrary::ClampAxis(UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(Dif.X, Dif.Y)));
}

void UInventoryWheelWidget::Follow(FVector2D Target)
{
	FVector2D Dir = Target - End;
	MouseAngle = UKismetMathLibrary::Vector_HeadingAngle(FVector(Dir.X, Dir.Y, 1.0f));
	UKismetMathLibrary::Normalize2D(Dir);
	End = Target - (Dir * Length);
}

void UInventoryWheelWidget::CalculateB()
{
	float Dx = Length * UKismetMathLibrary::Cos(MouseAngle);
	float Dy = Length * UKismetMathLibrary::Sin(MouseAngle);
	Start.X = Dx + End.X;
	Start.Y = Dy + End.Y;
}

float UInventoryWheelWidget::GetCurrentSector(int& InCurrentSector)
{
	for (int i = 0; i < MinSectorBounds.Num(); i++)
	{
		if (IsBetweenAngles(CurrentSearchSectorAngle, MinSectorBounds[i], MaxSectorBounds[i]))
		{
			InCurrentSector = i;
			return UKismetMathLibrary::ClampAxis(MinSectorBounds[i] + (SectorSize / 2.0f));
		}
	}
	return 0.0f;
}

bool UInventoryWheelWidget::IsBetweenAngles(float InAngle, float InStart, float InEnd)
{
	float A = InEnd - InStart < 0.0f ? InEnd - InStart + 360.0f : InEnd - InStart;
	float B = InAngle - InStart < 0.0f ? InAngle - InStart + 360.0f : InAngle - InStart;
	return B < A;
}
