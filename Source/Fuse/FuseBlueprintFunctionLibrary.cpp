// Fill out your copyright notice in the Description page of Project Settings.


#include "FuseBlueprintFunctionLibrary.h"

#include "AbilitySystemComponent.h"

FGameplayTagContainer UFuseBlueprintFunctionLibrary::GetAssetTags(const TSubclassOf<UGameplayAbility> anAbilityClass)
{
	if (!anAbilityClass) {
		return {};
	}
	
	return anAbilityClass->GetDefaultObject<UGameplayAbility>()->GetAssetTags();
}

bool UFuseBlueprintFunctionLibrary::HasAbility(const UAbilitySystemComponent* const anAsc, const TSubclassOf<UGameplayAbility> anAbilityClass)
{
	return anAsc && anAbilityClass && anAsc->FindAbilitySpecFromClass(anAbilityClass);
}
