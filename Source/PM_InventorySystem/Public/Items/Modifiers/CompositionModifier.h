// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Items/Modifiers/ItemModifier.h"
#include "CompositionModifier.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCompositeItemHandledModifierEvent, UItemInstance*, ItemInstance);

/**
 * A Modifier for when an Item is made up of several parts.
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UCompositionModifier : public UItemModifier
{
	GENERATED_BODY()

public:
	virtual void InitializeItemModifier(UItemInstance* InItemInstance, UObject* InWorldContextObject = nullptr) override;

	UFUNCTION(BlueprintCallable)
	bool TryAddCompositeItem(UItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	UItemInstance* RemoveCompositeItem(FGameplayTag ItemToRemove, bool& bSuccess);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CompositeItemAdded(UItemInstance* AddedItem);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CompositeItemRemoved(UItemInstance* RemovedItem);

	// The Item Tags necessary for this composite to function. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Item"))
	FGameplayTagContainer CompositeItemTags;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, UItemInstance*> CompositeItems;

	UPROPERTY(BlueprintAssignable)
	FCompositeItemHandledModifierEvent OnCompositeItemAdded;

	UPROPERTY(BlueprintAssignable)
	FCompositeItemHandledModifierEvent OnCompositeItemRemoved;
};
