// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Modifiers/CompositionModifier.h"

#include "Items/ItemDefinition.h"

void UCompositionModifier::InitializeItemModifier(UItemInstance* InItemInstance, UObject* InWorldContextObject)
{
	Super::InitializeItemModifier(InItemInstance, InWorldContextObject);

	
}

bool UCompositionModifier::TryAddCompositeItem(UItemInstance* InItemInstance)
{
	if (!InItemInstance)
	{
		return false;
	}

	FGameplayTag ItemTag = InItemInstance->GetItemDefinition()->ItemTag;
	if (!CompositeItemTags.HasTag(ItemTag))
	{
		return false;
	}

	CompositeItems.Add(ItemTag, InItemInstance);
	OnCompositeItemAdded.Broadcast(InItemInstance);
	CompositeItemAdded(InItemInstance);
	return true;

	
}

UItemInstance* UCompositionModifier::RemoveCompositeItem(FGameplayTag ItemToRemove, bool& bSuccess)
{
	UItemInstance* RemovedItem = CompositeItems.FindAndRemoveChecked(ItemToRemove);
	if (!RemovedItem)
	{
		bSuccess = false;
		return nullptr;
	}
	bSuccess = true;
	OnCompositeItemRemoved.Broadcast(RemovedItem);
	CompositeItemRemoved(RemovedItem);
	return RemovedItem;
}