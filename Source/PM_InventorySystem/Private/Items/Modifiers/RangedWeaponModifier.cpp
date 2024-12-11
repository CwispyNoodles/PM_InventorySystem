// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Modifiers/RangedWeaponModifier.h"

#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Ammo, "Item.Ammo");

void URangedWeaponModifier::Tick(float InDeltaTime)
{
	const bool bMinSpread = UpdateSpread(InDeltaTime);
	// const bool bMinMultipliers = UpdateMultipliers(InDeltaTime);

	bHasFirstShotAccuracy = bAllowFirstShotAccuracy && /* bMinMultipliers && */ bMinSpread;
}

void URangedWeaponModifier::InitializeItemModifier(UItemInstance* InItemInstance, UObject* InWorldContextObject)
{
	Super::InitializeItemModifier(InItemInstance, InWorldContextObject);

	CurrentMagazineCapacity = DefaultMagazineCapacity;
}

void URangedWeaponModifier::AddSpread()
{
	// Sample the heat up curve
	const float HeatPerShot = HeatToHeatPerShotCurve.GetRichCurveConst()->Eval(CurrentHeat);
	CurrentHeat = ClampHeat(CurrentHeat + HeatPerShot);

	// Map the heat to the spread angle
	CurrentSpreadAngle = HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);
}

int URangedWeaponModifier::GetMagazineAmmoDelta()
{
	if (AmmoItemInstance)
	{
		return CurrentMagazineCapacity - AmmoItemInstance->GetCurrentStack();
	}
	return CurrentMagazineCapacity;
}

bool URangedWeaponModifier::UpdateSpread(float DeltaSeconds)
{
	const float TimeSinceFired = GetWorld()->TimeSince(LastFireTime);

	if (TimeSinceFired > SpreadRecoveryCooldownDelay)
	{
		const float CooldownRate = HeatToCoolDownPerSecondCurve.GetRichCurveConst()->Eval(CurrentHeat);
		CurrentHeat = ClampHeat(CurrentHeat - (CooldownRate * DeltaSeconds));
		CurrentSpreadAngle = HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);
	}
	
	float MinSpread;
	float MaxSpread;
	ComputeSpreadRange(/*out*/ MinSpread, /*out*/ MaxSpread);

	return FMath::IsNearlyEqual(CurrentSpreadAngle, MinSpread, KINDA_SMALL_NUMBER);
}

bool URangedWeaponModifier::UpdateMultipliers(float DeltaSeconds)
{
	return false;
}

void URangedWeaponModifier::ComputeSpreadRange(float& MinSpread, float& MaxSpread)
{
	HeatToSpreadCurve.GetRichCurveConst()->GetValueRange(/*out*/ MinSpread, /*out*/ MaxSpread);
}

void URangedWeaponModifier::ComputeHeatRange(float& MinHeat, float& MaxHeat)
{
	float Min1;
	float Max1;
	HeatToHeatPerShotCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min1, /*out*/ Max1);

	float Min2;
	float Max2;
	HeatToCoolDownPerSecondCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min2, /*out*/ Max2);

	float Min3;
	float Max3;
	HeatToSpreadCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min3, /*out*/ Max3);

	MinHeat = FMath::Min(FMath::Min(Min1, Min2), Min3);
	MaxHeat = FMath::Max(FMath::Max(Max1, Max2), Max3);
}
