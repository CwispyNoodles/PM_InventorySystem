// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Modifiers/EquipmentModifier.h"

#include "Items/ItemActor.h"

AItemActor* UEquipmentModifier::SpawnEquipmentActor(FTransform SpawnTransform)
{
	if (!IsValid(EquipmentItemActor))
	{
		EquipmentItemActor = ItemReference->SpawnItemActor(SpawnTransform);
	}
	return EquipmentItemActor;

}

