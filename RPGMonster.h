// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "RPGMonster.generated.h"

UCLASS()
class RPGPROJECT_API ARPGMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGMonster();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);
	void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	void OnDieAnimationEnd();

	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0.f) override;

	void StartAttack();
	void StopAttack();


	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MosterMesh;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* MosterCollision;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* WeaponCollision;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* BeHitAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DeathAnim;

	UPROPERTY(EditDefaultsOnly, Category = Behavior)
	class UBehaviorTree* MonsterBehavior;

	UFUNCTION(BlueprintPure, Category = "Health")
	float Getcurrenthealth();

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class AItem> ItemClass;

private:

	bool bAttacking;
	bool bAttackComplite;
	float health;

	FTimerHandle TimerHandle_EnemyStopAttack;
	
};
