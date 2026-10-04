// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FUSE_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData myMaxWalkSpeed;
	
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPlayerAttributeSet, myMaxWalkSpeed)
};
