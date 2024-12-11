// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ItemAbility.h"

#include "InventoryManagerComponent.h"
#include "Items/ItemInstance.h"

UItemInstance* UItemAbility::GetAssociatedItem()
{
	if (FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec())
	{
		return Cast<UItemInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

UInventoryData* UItemAbility::GetOwnerInventoryData(bool& bSuccess)
{
	FGameplayAbilityActorInfo OwnerInfo = GetActorInfo();
	if (UInventoryManagerComponent* IMC = OwnerInfo.AvatarActor->GetComponentByClass<UInventoryManagerComponent>())
	{
		bSuccess = true;
		return IMC->GetInventoryData();
	}
	bSuccess = false;
	return nullptr;
}
