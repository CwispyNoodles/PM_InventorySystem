// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemInstance.generated.h"

class UItemModifier;
class AItemActor;
class UItemDefinition;

UENUM(BlueprintType)
enum EItemWidgetDropState
{
	VALID		UMETA(DisplayName="Valid"),
	INVALID		UMETA(DisplayName="Invalid"),
	OBSTRUCTED	UMETA(DisplayName="Obstructed")
};

DECLARE_MULTICAST_DELEGATE(FOnRefreshSignature);
DECLARE_MULTICAST_DELEGATE(FOnDirtySignature);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnRemoveSignature, UItemInstance*, ItemToRemove);

UCLASS(BlueprintType)
class PM_INVENTORYSYSTEM_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	bool InitializeItemInstance(UItemDefinition* InItemDefinition, UObject* InWorldContextObject, UObject* NewOwner = nullptr);
	virtual UWorld* GetWorld() const override;

	// // IPCContextMenuInterface
	// virtual UPCContextMenuData* GetContextMenuData(bool& bSuccess) override;
	// virtual UPCContextMenuAction* GetDefaultAction() override;
	// // End IPCContextMenuInterface

	UFUNCTION(BlueprintCallable)
	void SetOwner(UObject* NewOwner) { Owner = NewOwner; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	TArray<UItemModifier*> GetItemModifiers() { return ItemModifiers; }

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(DeterminesOutputType="InModifierClass"))
	UItemModifier* GetItemModifierByClass(TSubclassOf<UItemModifier> InModifierClass, bool& bSuccess);
	
	UFUNCTION()
	UItemModifier* FindItemModifierByClass(TSubclassOf<UItemModifier> ItemModifierClass) const;

	template <typename ResultClass>
	ResultClass* FindItemModifierByClass()
	{
		return (ResultClass*)FindItemModifierByClass(ResultClass::StaticClass());
	}

	UFUNCTION(BlueprintCallable)
	void DeclareDirty();

	UFUNCTION(BlueprintCallable)
	bool RemoveFromInventory();

	UFUNCTION(BlueprintCallable)
	AItemActor* SpawnItemActor(FTransform SpawnTransform = FTransform());

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UObject* GetOwner() { return Owner; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<AItemActor> GetItemActorClass();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntPoint GetDimensions();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UItemDefinition* GetItemDefinition() { return ItemDefinition; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCurrentStack() { return CurrentStack; }

	UFUNCTION(BlueprintCallable)
	EItemWidgetDropState CanDrop(UItemInstance* ItemToDrop);

	UFUNCTION(BlueprintCallable)
	void SetStack(int32 NewStack);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMaxStack();

	UFUNCTION(BlueprintCallable)
	void Rotate();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AItemActor*> GetSpawnedItemActors();

	UFUNCTION(BlueprintCallable)
	void DestroyAllSpawnedActors();

	UFUNCTION(BlueprintCallable)
	void RegisterItemActor(AItemActor* NewItemActor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsRotated() { return bIsRotated; }

	UFUNCTION(BlueprintCallable)
	void RefreshItemThumbnail(UTexture2D* NewItemThumbnail);
	
	/**
	 * @param AddStack The amount you want to add to the current stack.
	 * @param MaxDif (AddStack + CurrentStack) - MaxStack
	 * @return Whether or not the AddStack causes the current stack to overflow past the MaxStack. 
	 */
	UFUNCTION(BlueprintCallable)
	bool AddStack(int32 AddStack, int32& MaxDif);

	UFUNCTION(BlueprintCallable)
	bool AddStackWithinBound(int32 AddStack, int32 Bound, int32& MaxDif);

	// Returns true if amount exceeds current stack.
	UFUNCTION(BlueprintCallable)
	bool Transfer(int32 Amount, UItemInstance*& NewTransferAmount);

	FOnDirtySignature DirtyDelegate;
	FOnRefreshSignature RefreshDelegate;
	FOnRemoveSignature RemoveDelegate;
private:
	UFUNCTION()
	void RegisterItemModifier(UItemModifier* InItemModifier, UObject* InWorldContextObject);

	UFUNCTION()
	void RemoveItemActor(AActor* DestroyedActor);

	UPROPERTY()
	TArray<AItemActor*> ItemActors;
	
	UPROPERTY()
	UObject* WorldContext;

	UPROPERTY()
	UObject* Owner;

	UPROPERTY()
	TObjectPtr<UItemDefinition> ItemDefinition;

	// UPROPERTY()
	// TObjectPtr<UPCContextMenuData> ContextMenuData;
	//
	// UPROPERTY()
	// TObjectPtr<UPCContextMenuAction> DefaultAction;

	UPROPERTY()
	TObjectPtr<UTexture2D> CachedItemThumbnail;

	UPROPERTY()
	bool bIsRotated;

	UPROPERTY()
	TArray<UItemModifier*> ItemModifiers;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int32 CurrentStack = 1;
};
