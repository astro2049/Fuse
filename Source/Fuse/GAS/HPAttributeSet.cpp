// Fill out your copyright notice in the Description page of Project Settings.


#include "HPAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

bool UHPAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetmyHPAttribute() && myIsDead) {
		return false;
	}
	
	return Super::PreGameplayEffectExecute(Data);
}

void UHPAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute != GetmyHPAttribute()) {
		return;
	}
	
	if (myHP.GetCurrentValue() <= 0.f) {
		SetmyHP(0.f);
		myIsDead = true;
		OnDead.Broadcast();
	}
}
