// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PublicCharacter.generated.h"

UCLASS()
class RPGPROJECT_API APublicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APublicCharacter();

	
	USkeletalMeshComponent* GetSpecificPawnMesh() const;
	FName GetWeaponAttachPoint() const;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FName WeaponAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class ASimpleWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* BeHitAnim;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DeathAnim;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* BeBlockAnim;

	class ASimpleWeapon* Weapon;

	void SetWeapon();

	virtual void WeaponAttack();

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);
	void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);
	void OnDieAnimationEnd();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float Getcurrenthealth();

	float fHealth;
};
