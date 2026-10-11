#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PerkData.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UCLASS(BlueprintType, Blueprintable)
class FUSE_API UPerkData : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Fuse")
	int32 GetMaxLevel() const { return myDescriptions.Num() - 1; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText myName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FText> myDescriptions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor myNameColor = FLinearColor::White;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> myGameplayEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> myGameplayAbility;
};
