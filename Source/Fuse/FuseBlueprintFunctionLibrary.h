// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "FuseBlueprintFunctionLibrary.generated.h"

UCLASS()
class FUSE_API UFuseBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure)
	static FGameplayTagContainer GetAssetTags(const TSubclassOf<UGameplayAbility> anAbilityClass);
	
	UFUNCTION(BlueprintPure)
	static bool HasAbility(const UAbilitySystemComponent* const anAsc, const TSubclassOf<UGameplayAbility> anAbilityClass);
};
