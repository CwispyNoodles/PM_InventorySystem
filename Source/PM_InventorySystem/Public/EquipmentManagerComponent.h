// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "EquipmentManagerComponent.generated.h"

struct FAbilitySet_GrantedHandles;

/*
 * NoteToSelf: The reason I decided to make the EquipmentManagerComponent distinct from the InventoryManagerComponent
 * is because not all actors with inventories are able to "equip" items. For example, a box may have an inventory but
 * would not be able to equip an item. Whereas, a character is able to both have an inventory and equip items.
 */

class UItemInstance;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentStateChangeSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PM_INVENTORYSYSTEM_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentManagerComponent();
	virtual void InitializeComponent() override;

	UFUNCTION(BlueprintCallable)
	void ChangeEquipmentSlot(FGameplayTag SlotActionTag);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetActiveEquipmentTag(FGameplayTag InEquipmentTag);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetActiveEquipmentTag() { return ActiveEquipmentTag; }

	UFUNCTION(BlueprintCallable)
	void EquipItem(UItemInstance* ItemToEquip);

	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	void UnequipItem_Internal();
	void EquipItem_Internal();

	// Referenced by Equipment Items whenever the parent actor unequips. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations")
	TSubclassOf<UAnimInstance> DefaultAnimationLayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, USkeletalMesh*> DefaultMeshes;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentStateChangeSignature OnEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentStateChangeSignature OnUnequipDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentStateChangeSignature OnInterruptDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentStateChangeSignature OnSwapDelegate;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemInstance> EquippedItem;

private:
	UPROPERTY()
	FGameplayTag ActiveEquipmentTag;

	UPROPERTY()
	TArray<FAbilitySet_GrantedHandles> GrantedAbilityHandles; 
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
