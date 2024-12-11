// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RangedWeaponAbility.h"

#include "AbilitySystemComponent.h"
#include "Items/Modifiers/RangedWeaponModifier.h"
#include "Kismet/KismetSystemLibrary.h"

void FPCGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context,
                                                                          bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);
}

bool FPCGameplayAbilityTargetData_SingleTargetHit::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);
	
	Ar << CartridgeID;

	return true;
}

void URangedWeaponAbility::StartRangedWeaponTargeting()
{
	check(CurrentActorInfo);
	
	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);
	
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);
	
	FScopedPredictionWindow ScopedPrediction(MyAbilityComponent, CurrentActivationInfo.GetActivationPredictionKey());
	
	TArray<FHitResult> FoundHits;
	PerformTargeting(/*out*/ FoundHits);
	
	// Fill out the target data from the hit results
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData.UniqueId = FMath::Rand();
	
	if (FoundHits.Num() > 0)
	{
		const int32 CartridgeID = FMath::Rand();
	
		for (const FHitResult& FoundHit : FoundHits)
		{
			FPCGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FPCGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			NewTargetData->CartridgeID = CartridgeID;
	
			TargetData.Add(NewTargetData);
		}
	}
	
	// Process the target data immediately
	OnTargetDataReadyCallback(TargetData, FGameplayTag());
}

void URangedWeaponAbility::PerformTargeting(TArray<FHitResult>& OutHits)
{
	UItemInstance* Item = GetAssociatedItem();
	check(Item);
	
	if (URangedWeaponModifier* RangedWeapon = Item->FindItemModifierByClass<URangedWeaponModifier>())
	{
		FRangedWeaponFiringInput InputData;
		InputData.WeaponData = RangedWeapon;
		InputData.TargetData = GetTargetData();
	
		TraceBulletsInCartridge(InputData, OutHits);
	}
}

FHitResult URangedWeaponAbility::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace,
	float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const
{
	FHitResult Impact;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	TArray<AActor*> ActorsToIgnore;
	// ActorsToIgnore.Add(UGameplayStatics::GetPlayerPawn(this, 0));

	if (bDebugTrace)
	{
		UKismetSystemLibrary::LineTraceSingleForObjects(this, StartTrace, EndTrace, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Impact, true);
	}
	else
	{
		UKismetSystemLibrary::LineTraceSingleForObjects(this, StartTrace, EndTrace, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::None, Impact, true);
	}
	
	OutHits.Add(Impact);
	return Impact;
}

FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.f)
	{
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);

		// consider the cone a concatenation of two rotations. one "away" from the center line, and another "around" the circle
		// apply the exponent to the away-from-center rotation. a larger exponent will cluster points more tightly around the center
		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
		const float AngleAround = FMath::FRand() * 360.0f;

		FRotator Rot = Dir.Rotation();
		FQuat DirQuat(Rot);
		FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
		FQuat AroundQuat(FRotator(0.0f, 0.0, AngleAround));
		FQuat FinalDirectionQuat = DirQuat * AroundQuat * FromCenterQuat;
		FinalDirectionQuat.Normalize();

		return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
	}
	else
	{
		return Dir.GetSafeNormal();
	}
}

void URangedWeaponAbility::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData,
	TArray<FHitResult>& OutHits)
{
	URangedWeaponModifier* WeaponData = InputData.WeaponData;
	check(WeaponData);
	
	const int32 BulletsPerCartridge = WeaponData->GetBulletsPerCartridge();
	
	for (int32 BulletIndex = 0; BulletIndex < BulletsPerCartridge; ++BulletIndex)
	{
		const float BaseSpreadAngle = WeaponData->GetCalculatedSpreadAngle();
		const float SpreadAngleMultiplier = WeaponData->GetCalculatedSpreadAngleMultiplier();
		const float ActualSpreadAngle = BaseSpreadAngle * SpreadAngleMultiplier;
	
		const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(ActualSpreadAngle * 0.5f);
	
		const FVector BulletDir = VRandConeNormalDistribution(InputData.TargetData.AimDir, HalfSpreadAngleInRadians, WeaponData->GetSpreadExponent());
	
		const FVector EndTrace = InputData.TargetData.StartTrace + (BulletDir * WeaponData->GetMaxDamageRange());
	
		TArray<FHitResult> AllImpacts;
	
		FHitResult Impact = DoSingleBulletTrace(InputData.TargetData.StartTrace, EndTrace, WeaponData->GetBulletTraceSweepRadius(), /*bIsSimulated=*/ false, /*out*/ AllImpacts);
	
		const AActor* HitActor = Impact.GetActor();
	
		if (HitActor)
		{
			if (AllImpacts.Num() > 0)
			{
				OutHits.Append(AllImpacts);
			}
		}
	
		// Make sure there's always an entry in OutHits so the direction can be used for tracers, etc...
		if (OutHits.Num() == 0)
		{
			if (!Impact.bBlockingHit)
			{
				// Locate the fake 'impact' at the end of the trace
				Impact.Location = EndTrace;
				Impact.ImpactPoint = EndTrace;
			}
	
			OutHits.Add(Impact);
		}
	}
}

void URangedWeaponAbility::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData,
	FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);
	
	if (const FGameplayAbilitySpec* AbilitySpec = MyAbilityComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FScopedPredictionWindow	ScopedPrediction(MyAbilityComponent);
	
		// Take ownership of the target data to make sure no callbacks into game code invalidate it out from under us
		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));
	
		// We fired the weapon, add spread
		UItemInstance* Item = GetAssociatedItem();
		check(Item);
	
		if (URangedWeaponModifier* RangedWeapon = Item->FindItemModifierByClass<URangedWeaponModifier>())
		{
			RangedWeapon->AddSpread();
		}
	
		// Let the blueprint do stuff like apply effects to the targets
		OnRangedWeaponTargetDataReady(LocalTargetDataHandle);
	}
}
