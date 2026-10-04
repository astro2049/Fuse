// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HPAttributeSet.generated.h"

UCLASS()
class FUSE_API UHPAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData myHP;
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData myMaxHP;

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UHPAttributeSet, myHP)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(myHP)
};
