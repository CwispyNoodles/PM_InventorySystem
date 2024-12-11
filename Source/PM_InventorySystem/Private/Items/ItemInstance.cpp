// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemInstance.h"

#include "Items/ItemActor.h"
#include "Items/ItemDefinition.h"

bool UItemInstance::InitializeItemInstance(UItemDefinition* InItemDefinition, UObject* InWorldContextObject, UObject* NewOwner)
{
	check(InWorldContextObject->GetWorld())
	check(InItemDefinition);

	if (IsValid(NewOwner))
	{
		Owner = NewOwner;
	}

	WorldContext = InWorldContextObject;
	ItemDefinition = InItemDefinition;

	for (UItemModifier* ItemModifier : ItemDefinition->Modifiers)
	{
		RegisterItemModifier(ItemModifier, InWorldContextObject);
	}
	// if (UPCContextMenuActionSet* ContextMenuActionSet = ItemDefinition->ItemContextMenuActionSet)
	// {
	// 	ContextMenuData = ContextMenuActionSet->CreateNewContextMenuData();
	// }
	// if (TSubclassOf<UPCContextMenuAction> DefaultActionClass = ItemDefinition->DefaultActionClass)
	// {
	// 	DefaultAction = NewObject<UPCContextMenuAction>(this, DefaultActionClass);
	// 	DefaultAction->InitializeContextMenuAction(this);
	// }
	//
	// for (TSubclassOf<UPCItemModule> ItemModule : ItemDefinition->Modules)
	// {
	// 	RegisterItemModule(ItemModule, InWorldContextObject);
	// }
	
	return true;
}

UWorld* UItemInstance::GetWorld() const
{
	return WorldContext->GetWorld();
}

UItemModifier* UItemInstance::GetItemModifierByClass(TSubclassOf<UItemModifier> InModifierClass, bool& bSuccess)
{
	for (UItemModifier* ItemModifier : ItemModifiers)
	{
		if (ItemModifier->IsA(InModifierClass))
		{
			bSuccess = true;
			return ItemModifier;
		}
	}
	bSuccess = false;
	return nullptr;
}

UItemModifier* UItemInstance::FindItemModifierByClass(TSubclassOf<UItemModifier> ItemModifierClass) const
{
	if (ItemModifierClass != nullptr)
	{
		for (UItemModifier* ItemModifier : ItemModifiers)
		{
			if (ItemModifier != nullptr && ItemModifier->IsA(ItemModifierClass))
			{
				return ItemModifier;
			}
		}
	}

	return nullptr;
}

void UItemInstance::DeclareDirty()
{
	DirtyDelegate.Broadcast();
	RefreshDelegate.Broadcast();
}

bool UItemInstance::RemoveFromInventory()
{
	if (RemoveDelegate.IsBound())
	{
		return RemoveDelegate.Execute(this);
	}
	return false;
}

AItemActor* UItemInstance::SpawnItemActor(FTransform SpawnTransform)
{
	AItemActor* NewItemActor = Cast<AItemActor>(GetWorld()->SpawnActor(GetItemActorClass(), &SpawnTransform));
	if (IsValid(NewItemActor))
	{
		// NewItemActor->InitializeItemActor(this);
		RegisterItemActor(NewItemActor);
		
		TArray<UPrimitiveComponent*> UPrimitiveComponents;
		NewItemActor->GetComponents<UPrimitiveComponent>(UPrimitiveComponents);
		if (IsValid(GetOwner()))
		{
			for (UPrimitiveComponent* UPrimitiveComponent : UPrimitiveComponents)
			{
				UPrimitiveComponent->SetCollisionProfileName(FName("NoCollision"));
			}
		}
		else
		{
			// UPCStatics::CreateComponentOnInstance<UPCInteractComponent>(NewItemActor);
			for (UPrimitiveComponent* UPrimitiveComponent : UPrimitiveComponents)
			{
				UPrimitiveComponent->SetCollisionProfileName(FName("Item"));
			}
			if (UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(NewItemActor->GetRootComponent()))
			{
				Root->SetSimulatePhysics(true);
			}
		}
		
	}
	return NewItemActor;
}

TSubclassOf<AItemActor> UItemInstance::GetItemActorClass()
{
	if (IsValid(Owner))
	{
		return ItemDefinition->InventoryActorClass;
	}
	return ItemDefinition->WorldActorClass;
}

FIntPoint UItemInstance::GetDimensions()
{
	const FIntPoint Dimensions = ItemDefinition->Dimensions;
	if (bIsRotated == true)
	{
		return FIntPoint(Dimensions.Y, Dimensions.X);
	}
	return Dimensions;
}

EItemWidgetDropState UItemInstance::CanDrop(UItemInstance* ItemToDrop)
{
	if (ItemDefinition->ItemTag.MatchesTag(ItemToDrop->GetItemDefinition()->ItemTag) && ItemToDrop != this)
	{
		if (CurrentStack < ItemDefinition->MaxStackAmount)
		{
			return VALID;
		}
		return OBSTRUCTED;
	}
	return INVALID;
}

void UItemInstance::SetStack(int32 NewStack)
{
	CurrentStack = FMath::Clamp(NewStack, 1, ItemDefinition->MaxStackAmount);
	RefreshDelegate.Broadcast();
}

bool UItemInstance::IsMaxStack()
{
	return CurrentStack == ItemDefinition->MaxStackAmount;
}

void UItemInstance::Rotate()
{
	const FIntPoint Dimensions = ItemDefinition->Dimensions;
	if (Dimensions.X != Dimensions.Y)
	{
		bIsRotated = !bIsRotated;
		RefreshDelegate.Broadcast();
	}
}

TArray<AItemActor*> UItemInstance::GetSpawnedItemActors()
{
	return ItemActors;
}

void UItemInstance::DestroyAllSpawnedActors()
{
	while (ItemActors.Num() > 0)
    {
    	ItemActors[0]->Destroy();
    }
    ItemActors.Empty();
}

void UItemInstance::RegisterItemActor(AItemActor* NewItemActor)
{
	NewItemActor->OnDestroyed.AddDynamic(this, &ThisClass::RemoveItemActor);
	ItemActors.AddUnique(NewItemActor);
}

void UItemInstance::RefreshItemThumbnail(UTexture2D* NewItemThumbnail)
{
	CachedItemThumbnail = NewItemThumbnail;
	RefreshDelegate.Broadcast();
}

bool UItemInstance::AddStack(int32 AddStack, int32& MaxDif)
{
	const int32 Sum = AddStack + CurrentStack;
	CurrentStack = FMath::Clamp(Sum, 1, ItemDefinition->MaxStackAmount);
	RefreshDelegate.Broadcast();
	if (Sum > ItemDefinition->MaxStackAmount)
	{
		MaxDif = Sum - ItemDefinition->MaxStackAmount;
		return true;
	}
	MaxDif = 0;
	return false; 
}

bool UItemInstance::AddStackWithinBound(int32 AddStack, int32 Bound, int32& MaxDif)
{
	const int32 Sum = AddStack + CurrentStack;
	CurrentStack = FMath::Clamp(Sum, 1, Bound);
	RefreshDelegate.Broadcast();
	if (Sum > Bound)
	{
		MaxDif = Sum - Bound;
		return true;
	}
	MaxDif = 0;
	return false;
}

bool UItemInstance::Transfer(int32 Amount, UItemInstance*& NewTransferAmount)
{
	if (Amount >= CurrentStack)
	{
		NewTransferAmount = this;
		return true;
	}

	NewTransferAmount = DuplicateObject(this, nullptr);
	SetStack(CurrentStack - Amount);
	NewTransferAmount->SetStack(Amount);
	return false;
}

void UItemInstance::RegisterItemModifier(UItemModifier* InItemModifier, UObject* InWorldContextObject)
{
	// We duplicate the Item Modifier because all instances of this item would share the same Modifier instance otherwise.
	UItemModifier* NewItemModifier = DuplicateObject(InItemModifier, this);
	check(NewItemModifier);

	NewItemModifier->InitializeItemModifier(this, InWorldContextObject);
	ItemModifiers.AddUnique(NewItemModifier);
}

void UItemInstance::RemoveItemActor(AActor* DestroyedActor)
{
	if (AItemActor* DestroyedItem = Cast<AItemActor>(DestroyedActor))
	{
		ItemActors.Remove(DestroyedItem);
	}
}
