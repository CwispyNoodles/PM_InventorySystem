// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagerComponent.h"

#include "AbilitySet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintGameplayTagLibrary.h"
#include "NativeGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Items/Modifiers/EquipmentModifier.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Ability_EquipmentSlot, "InputTag.Ability.EquipmentSlot");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Ability_EquipmentSlot_Unarmed, "InputTag.Ability.EquipmentSlot.Unarmed");

// Sets default values for this component's properties
UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
}

void UEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	ActiveEquipmentTag = TAG_InputTag_Ability_EquipmentSlot_Unarmed;
}

void UEquipmentManagerComponent::ChangeEquipmentSlot(FGameplayTag SlotActionTag)
{
	FGameplayEventData Payload = FGameplayEventData();
	Payload.InstigatorTags = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(SlotActionTag);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), TAG_InputTag_Ability_EquipmentSlot, Payload);
}

void UEquipmentManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (EquippedItem)
	{
		if (UEquipmentModifier* EquipmentModifier = EquippedItem->FindItemModifierByClass<UEquipmentModifier>())
		{
			EquipmentModifier->Tick(DeltaTime);
		}
	}
}

void UEquipmentManagerComponent::SetActiveEquipmentTag(FGameplayTag InEquipmentTag)
{
	if (InEquipmentTag == ActiveEquipmentTag)
	{
		UnequipItem_Internal();
	}
	else if (ActiveEquipmentTag == TAG_InputTag_Ability_EquipmentSlot_Unarmed)
	{
		ActiveEquipmentTag = InEquipmentTag;
		EquipItem_Internal();
	}
	else
	{
		ActiveEquipmentTag = InEquipmentTag;
		OnInterruptDelegate.Broadcast();
	}
}

void UEquipmentManagerComponent::EquipItem(UItemInstance* ItemToEquip)
{
	check(ItemToEquip)

	UPMAbilitySystemComponent* ASC = GetOwner()->GetComponentByClass<UPMAbilitySystemComponent>();
	check(ASC);

	for (auto ItemModifier : ItemToEquip->GetItemModifiers())
	{
		if (UEquipmentModifier* EquipmentModifier = Cast<UEquipmentModifier>(ItemModifier))
		{
			for (auto AbilitySet : EquipmentModifier->AbilitySetsToGrant)
			{
				GrantedAbilityHandles.Add(AbilitySet->GiveToAbilitySystem(ASC, ItemToEquip));
			}
		}
	}
	
	EquippedItem = ItemToEquip;
}

void UEquipmentManagerComponent::UnequipItem()
{
	UPMAbilitySystemComponent* ASC = GetOwner()->GetComponentByClass<UPMAbilitySystemComponent>();
	check(ASC);
	
	for (FAbilitySet_GrantedHandles GrantedAbilityHandle : GrantedAbilityHandles)
	{
		GrantedAbilityHandle.TakeFromAbilitySystem(ASC);
	}

	EquippedItem = nullptr;
}

void UEquipmentManagerComponent::UnequipItem_Internal()
{
	ActiveEquipmentTag = TAG_InputTag_Ability_EquipmentSlot_Unarmed;
	OnUnequipDelegate.Broadcast();
}

void UEquipmentManagerComponent::EquipItem_Internal()
{
	OnEquipDelegate.Broadcast();
}

void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}


