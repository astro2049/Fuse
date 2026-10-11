// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "FuseBlueprintFunctionLibrary.generated.h"

class UAbilitySystemComponent;

UCLASS()
class FUSE_API UFuseBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Fuse|GAS")
	static FGameplayTagContainer GetAssetTags(const TSubclassOf<UGameplayAbility> anAbilityClass);
	
	UFUNCTION(BlueprintPure, Category = "Fuse|GAS")
	static bool HasAbility(const UAbilitySystemComponent* const anAsc, const TSubclassOf<UGameplayAbility> anAbilityClass);
};
