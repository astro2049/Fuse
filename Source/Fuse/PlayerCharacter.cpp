// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/PlayerAttributeSet.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UAbilitySystemComponent* asc = GetComponentByClass<UAbilitySystemComponent>()) {
		const FGameplayAttribute attribute = UPlayerAttributeSet::GetmyMaxWalkSpeedAttribute();
		asc->GetGameplayAttributeValueChangeDelegate(attribute)
			.AddUObject(this, &APlayerCharacter::OnMaxWalkSpeedChanged);
		
		GetCharacterMovement()->MaxWalkSpeed = asc->GetNumericAttribute(attribute);
	}
}

void APlayerCharacter::OnMaxWalkSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GetCharacterMovement()->MaxWalkSpeed = OnAttributeChangeData.NewValue;
}
