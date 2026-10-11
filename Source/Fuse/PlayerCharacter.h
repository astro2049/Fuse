// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UGameplayEffect;
class UPerkData;

UCLASS()
class FUSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Fuse")
    void OnPerksChanged();
	UFUNCTION(BlueprintCallable, Category="Fuse")
	void AddPerk(UPerkData* aPerk);
	UFUNCTION(BlueprintPure, Category="Fuse")
	int32 GetPerkLevel(const UPerkData* aPerk) const;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UPerkData>> myPerks;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPerkData> myFieldAbilityPerk;
	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle myFieldAbilitySpecHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	void OnMaxWalkSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
	
	bool AddPerkEffect(UPerkData* aPerk);
	bool AddPerkAbility(UPerkData* aPerk);
	
	// Utility
	/** Finds the first active effect of this class. Perks should have one active effect per class. */
	const FActiveGameplayEffect* FindActiveEffectFromClass(TSubclassOf<UGameplayEffect> anEffectClass) const;
};
