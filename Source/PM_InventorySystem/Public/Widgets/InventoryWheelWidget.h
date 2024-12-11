// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonInventoryWidget.h"
#include "InventoryWheelWidget.generated.h"

class UInventoryWheelData;
class UItemDisplayWidget;
/**
 * 
 */ 
UCLASS(Abstract, Blueprintable)
class PM_INVENTORYSYSTEM_API UInventoryWheelWidget : public UCommonInventoryWidget
{
	GENERATED_BODY()

public:
	virtual UInventoryData* CreateNewInventoryData(UObject* Payload) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

	virtual void SetInventoryData(UInventoryData* NewInventoryData) override;

	virtual bool TryAddItem(UItemInstance* ItemToAdd) override;
	virtual void TryRemoveItem(UItemInstance* ItemToRemove) override;
	virtual void AddItemWidget(UItemInstance* NewItemInstance, FVector2D Pos) override;
	virtual void RemoveItemWidget(UItemInstance* NewItemInstance, FVector2D Pos) override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	int SetSelectedSector();
	
	UFUNCTION(BlueprintCallable)
	int GetItemNum() { return SectorNum; }

	// Handles material parameter values and other visuals.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Refresh();

	void NativeRefresh();

	UFUNCTION(BlueprintCallable)
	UInventoryWheelData* GetInventoryWheelData() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InventoryWheelRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Length = 75.0f;

private:
	void CalculateSectorBounds();
	float AngleBetweenVectors(FVector2D InA, FVector2D InB);
	void Follow(FVector2D Target);
	void CalculateB();
	float GetCurrentSector(int& InCurrentSector);
	bool IsBetweenAngles(float InAngle, float InStart, float InEnd);

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector2D End;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector2D Start;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector2D MouseDir;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector2D MousePos;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	float MouseAngle;

protected:
	UPROPERTY(BlueprintReadOnly)
	int CurrentActiveSector;

	UPROPERTY(BlueprintReadWrite)
	int CurrentSelectedSector;
	
	UPROPERTY(BlueprintReadOnly)
	int SectorNum;

	UPROPERTY(BlueprintReadOnly)
	float SectorSize;

	UPROPERTY(BlueprintReadOnly)
	TArray<float> MinSectorBounds;

	UPROPERTY(BlueprintReadOnly)
	TArray<float> MaxSectorBounds;

	UPROPERTY(BlueprintReadOnly)
	float CurrentSearchSectorAngle;

	UPROPERTY(BlueprintReadOnly)
	float CurrentActiveSectorAngle;

	UPROPERTY(BlueprintReadWrite)
	float CurrentSelectedSectorAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UItemDisplayWidget> ItemDisplayWidgetClass;
	

	
};
