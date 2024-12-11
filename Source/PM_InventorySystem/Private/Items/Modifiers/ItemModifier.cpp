// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Modifiers/ItemModifier.h"

void UItemModifier::InitializeItemModifier(UItemInstance* InItemInstance, UObject* InWorldContextObject)
{
	ItemReference = InItemInstance;
	WorldContextObject = InWorldContextObject;
}
