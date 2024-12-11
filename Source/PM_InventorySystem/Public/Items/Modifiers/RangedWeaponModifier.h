// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Modifiers/WeaponModifier.h"
#include "RangedWeaponModifier.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API URangedWeaponModifier : public UWeaponModifier
{
	GENERATED_BODY()

public:
	virtual void Tick(float InDeltaTime) override;
	
	virtual void InitializeItemModifier(UItemInstance* InItemInstance, UObject* InWorldContextObject = nullptr) override;
	
	void AddSpread();

	int32 GetBulletsPerCartridge() const
	{
		return BulletsPerCartridge;
	}
	
	/** Returns the current spread angle (in degrees, diametrical) */
	float GetCalculatedSpreadAngle() const
	{
		return CurrentSpreadAngle;
	}
	
	float GetCalculatedSpreadAngleMultiplier() const
	{
		return bHasFirstShotAccuracy ? 0.0f : CurrentSpreadAngleMultiplier;
	}
	
	float GetSpreadExponent() const
	{
		return SpreadExponent;
	}
	
	float GetMaxDamageRange() const
	{
		return MaxDamageRange;
	}
	
	float GetBulletTraceSweepRadius() const
	{
		return BulletTraceSweepRadius;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetMagazineAmmoDelta();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Fire(const TArray<FVector>& ImpactPositions, const TArray<FVector>& ImpactNormals, const TArray<TEnumAsByte<EPhysicalSurface>>& ImpactSurfaceTypes);

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemInstance> AmmoItemInstance; 

private:
	// Updates the spread and returns true if the spread is at minimum
	bool UpdateSpread(float DeltaSeconds);
	
	// Updates the multipliers and returns true if they are at minimum
	bool UpdateMultipliers(float DeltaSeconds);
	
	void ComputeSpreadRange(float& MinSpread, float& MaxSpread);
	void ComputeHeatRange(float& MinHeat, float& MaxHeat);
	
	inline float ClampHeat(float NewHeat)
	{
		float MinHeat;
		float MaxHeat;
		ComputeHeatRange(/*out*/ MinHeat, /*out*/ MaxHeat);
	
		return FMath::Clamp(NewHeat, MinHeat, MaxHeat);
	}
	
	// Do we currently have first shot accuracy?
	bool bHasFirstShotAccuracy = false;

	// The current spread angle (in degrees, diametrical)
	float CurrentSpreadAngle = 0.0f;

	// The current *combined* spread angle multiplier
	float CurrentSpreadAngleMultiplier = 1.0f;

	// Time since this weapon was last fired (relative to world time)
	double LastFireTime = 0.0;
	
	// The current heat
	float CurrentHeat = 0.0f;

	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged", meta=(Categories="Item.Ammo"))
	FGameplayTag AmmoTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged", meta=(ClampMin=0))
	int DefaultMagazineCapacity = 1;

	UPROPERTY(BlueprintReadOnly)
	int CurrentMagazineCapacity = 1;

	// Number of bullets to fire in a single cartridge (typically 1, but may be more for shotguns)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged")
	int32 BulletsPerCartridge = 1;

	// The maximum distance at which this weapon can deal damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged", meta=(ForceUnits=cm))
	float MaxDamageRange = 25000.0f;

	// The radius for bullet traces sweep spheres (0.0 will result in a line trace)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged", meta=(ForceUnits=cm))
	float BulletTraceSweepRadius = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Particles")
	TObjectPtr<UNiagaraSystem> TracerSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Particles")
	TObjectPtr<UNiagaraSystem> MuzzleFlashSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Particles")
	TObjectPtr<UNiagaraSystem> ShellEjectionSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Particles")
	TObjectPtr<UStaticMesh> ShellEjectMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Spread", meta=(ForceUnits=RPM))
	float RateOfFire = 60.0f;

	// Spread exponent, affects how tightly shots will cluster around the center line
	// when the weapon has spread (non-perfect accuracy). Higher values will cause shots
	// to be closer to the center (default is 1.0 which means uniformly within the spread range)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Spread", meta=(ClampMin=0.1))
	float SpreadExponent = 1.0f;

	// A curve that maps the heat to the spread angle
	// The X range of this curve typically sets the min/max heat range of the weapon
	// The Y range of this curve is used to define the min and maximum spread angle
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Spread")
	FRuntimeFloatCurve HeatToSpreadCurve;

	// A curve that maps the current heat to the amount a single shot will further 'heat up'
	// This is typically a flat curve with a single data point indicating how much heat a shot adds,
	// but can be other shapes to do things like punish overheating by adding progressively more heat.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Spread")
	FRuntimeFloatCurve HeatToHeatPerShotCurve;
	
	// A curve that maps the current heat to the heat cooldown rate per second
	// This is typically a flat curve with a single data point indicating how fast the heat
	// wears off, but can be other shapes to do things like punish overheating by slowing down
	// recovery at high heat.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Spread")
	FRuntimeFloatCurve HeatToCoolDownPerSecondCurve;

	// Should the weapon have perfect accuracy when both player and weapon spread are at their minimum value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Spread")
	bool bAllowFirstShotAccuracy = false;

	// Time since firing before spread cooldown recovery begins (in seconds)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ranged|Spread", meta=(ForceUnits=s))
	float SpreadRecoveryCooldownDelay = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TObjectPtr<UAnimMontage> WeaponReloadAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TObjectPtr<UAnimMontage> WeaponFireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TObjectPtr<UAnimMontage> CharacterReloadAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TObjectPtr<UAnimMontage> CharacterFireAnimation;
	
};
