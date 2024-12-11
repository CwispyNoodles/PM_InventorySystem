// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryStatics.h"

#include "InventoryDeveloperSettings.h"

const UInventoryDeveloperSettings* UInventoryStatics::GetInventoryDeveloperSettings()
{
	return GetDefault<UInventoryDeveloperSettings>();
}
