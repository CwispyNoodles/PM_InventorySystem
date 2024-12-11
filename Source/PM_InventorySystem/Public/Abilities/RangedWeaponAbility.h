// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/WeaponAbility.h"
#include "RangedWeaponAbility.generated.h"

class URangedWeaponModifier;

USTRUCT(BlueprintType)
struct FTargetData
{
	GENERATED_BODY()
	
	// Start of the trace
	UPROPERTY(BlueprintReadWrite)
	FVector StartTrace;
	
	// The direction of the trace if aim were perfect
	UPROPERTY(BlueprintReadWrite)
	FVector AimDir;

	FTargetData()
		: StartTrace(ForceInitToZero)
		, AimDir(ForceInitToZero)
	{
	}
};

USTRUCT()
struct FPCGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()

	FPCGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{ }

	virtual void AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const override;

	/** ID to allow the identification of multiple bullets that were part of the same cartridge */
	UPROPERTY()
	int32 CartridgeID;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FPCGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}
};

/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API URangedWeaponAbility : public UWeaponAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	bool bDebugTrace = false;

protected:
	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
	
	void PerformTargeting(OUT TArray<FHitResult>& OutHits);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	FTargetData GetTargetData();
	
	// Wrapper around WeaponTrace to handle trying to do a ray trace before falling back to a sweep trace if there were no hits and SweepRadius is above zero 
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, OUT TArray<FHitResult>& OutHits) const;
	
	struct FRangedWeaponFiringInput
	{
		// The weapon instance / source of weapon data
		URangedWeaponModifier* WeaponData = nullptr;

		FTargetData TargetData = FTargetData();
	};
	
	// Traces all of the bullets in a single cartridge
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, OUT TArray<FHitResult>& OutHits);
	
	// Called when target data is ready
	UFUNCTION(BlueprintImplementableEvent)
	void OnRangedWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);
	
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);
};
