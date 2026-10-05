// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HPAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadDelegate);

UCLASS()
class FUSE_API UHPAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnDeadDelegate OnDead;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData myHP;
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData myMaxHP;

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UHPAttributeSet, myHP)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(myHP)
	
private:
	bool myIsDead{false};
};
