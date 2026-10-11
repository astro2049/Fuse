// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "PerkData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/PlayerAttributeSet.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UAbilitySystemComponent* asc = GetComponentByClass<UAbilitySystemComponent>()) {
		const FGameplayAttribute attribute = UPlayerAttributeSet::GetmyMaxWalkSpeedAttribute();
		asc->GetGameplayAttributeValueChangeDelegate(attribute)
			.AddUObject(this, &APlayerCharacter::OnMaxWalkSpeedChanged);
		
		GetCharacterMovement()->MaxWalkSpeed = asc->GetNumericAttribute(attribute);
	}
}

void APlayerCharacter::OnMaxWalkSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GetCharacterMovement()->MaxWalkSpeed = OnAttributeChangeData.NewValue;
}

void APlayerCharacter::AddPerk(UPerkData* aPerk)
{
	if (!IsValid(aPerk)) {
		return;
	}
	
	if (AddPerkEffect(aPerk) || AddPerkAbility(aPerk)) {
		OnPerksChanged();
	}
}

bool APlayerCharacter::AddPerkEffect(UPerkData* const aPerk)
{
	UAbilitySystemComponent* const asc = GetComponentByClass<UAbilitySystemComponent>();
	if (!asc || !aPerk->myGameplayEffect) {
		return false;
	}

	const FActiveGameplayEffect* const effect = FindActiveEffectFromClass(aPerk->myGameplayEffect);
	if (!effect) {  // give gameplay effect
		asc->ApplyGameplayEffectToSelf(aPerk->myGameplayEffect->GetDefaultObject<UGameplayEffect>(), 0.f, asc->MakeEffectContext());
		myPerks.Add(aPerk);
		return true;
	} else {  // upgrade gameplay effect, if eligible
		const int32 level = FMath::TruncToInt(effect->Spec.GetLevel());
		if (level < aPerk->GetMaxLevel()) {
			asc->SetActiveGameplayEffectLevel(effect->Handle, level + 1);  // upgrade gameplay effect
			return true;
		}
		return false;
	}
}

bool APlayerCharacter::AddPerkAbility(UPerkData* const aPerk)
{
	UAbilitySystemComponent* const asc = GetComponentByClass<UAbilitySystemComponent>();
	if (!asc || !aPerk->myGameplayAbility) {
		return false;
	}

	FGameplayAbilitySpec* abilitySpec = asc->FindAbilitySpecFromClass(aPerk->myGameplayAbility);
	if (!abilitySpec) {  // give ability
		const FGameplayTagContainer& tags = aPerk->myGameplayAbility->GetDefaultObject<UGameplayAbility>()->GetAssetTags();
		const bool isAmmoMod = tags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Perk.AmmoMod")));
		const bool isFieldAbility = tags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Perk.FieldAbility")));
		if (isAmmoMod || isFieldAbility) {
			if (isAmmoMod) {
				const FGameplayAbilitySpecHandle abilityHandle = asc->GiveAbility(FGameplayAbilitySpec(aPerk->myGameplayAbility, 0));
				asc->TryActivateAbility(abilityHandle);
			}
			else if (isFieldAbility) {
				const FGameplayAbilitySpecHandle abilityHandle = asc->GiveAbility(FGameplayAbilitySpec(aPerk->myGameplayAbility, 0));
				if (myFieldAbilitySpecHandle.IsValid()) {
					asc->CancelAbilityHandle(myFieldAbilitySpecHandle);
					asc->ClearAbility(myFieldAbilitySpecHandle);
				}
				myPerks.Remove(myFieldAbilityPerk);
				myFieldAbilityPerk = aPerk;
				myFieldAbilitySpecHandle = abilityHandle;
			}
			myPerks.Add(aPerk);
			return true;
		}
		return false;
	} else {  // upgrade ability, if eligible
		if (abilitySpec->Level < aPerk->GetMaxLevel()) {
			abilitySpec->Level++;  // upgrade ability
			asc->MarkAbilitySpecDirty(*abilitySpec);
			return true;
		}
		return false;
	}
}

int32 APlayerCharacter::GetPerkLevel(const UPerkData* aPerk) const
{
	const UAbilitySystemComponent* const asc = GetComponentByClass<UAbilitySystemComponent>();
	if (!asc || !IsValid(aPerk)) {
		return -1;
	}

	if (aPerk->myGameplayEffect) {
		if (const FActiveGameplayEffect* effect = FindActiveEffectFromClass(aPerk->myGameplayEffect)) {
			return FMath::TruncToInt(effect->Spec.GetLevel());
		}
	}
	if (aPerk->myGameplayAbility) {
		if (const FGameplayAbilitySpec* spec = asc->FindAbilitySpecFromClass(aPerk->myGameplayAbility)) {
			return spec->Level;
		}
	}
	
	return -1;
}

const FActiveGameplayEffect* APlayerCharacter::FindActiveEffectFromClass(TSubclassOf<UGameplayEffect> anEffectClass) const
{
	const UAbilitySystemComponent* const asc = GetComponentByClass<UAbilitySystemComponent>();
	if (!asc || !anEffectClass) {
		return nullptr;
	}

	FGameplayEffectQuery query;
	query.EffectDefinition = anEffectClass;
	TArray<FActiveGameplayEffectHandle> GEHandles = asc->GetActiveEffects(query);
	if (!GEHandles.IsEmpty()) {
		return asc->GetActiveGameplayEffect(GEHandles[0]);
	}
	
	return nullptr;
}
