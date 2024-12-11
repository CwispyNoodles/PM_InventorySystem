// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryVerbMessage.h"

FString FInventoryVerbMessage::ToString() const
{
	FString HumanReadableMessage;
	FInventoryVerbMessage::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, this, /*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
	return HumanReadableMessage;
}
