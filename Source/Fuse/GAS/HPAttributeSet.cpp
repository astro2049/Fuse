// Fill out your copyright notice in the Description page of Project Settings.


#include "HPAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

void UHPAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute != GetmyHPAttribute()) {
		return;
	}
	
	SetmyHP(FMath::Clamp(myHP.GetCurrentValue(), 0.f, myMaxHP.GetCurrentValue()));
}
