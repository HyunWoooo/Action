// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PublicCharacter.h"
#include "SimpleWeapon.generated.h"

UCLASS()
class RPGPROJECT_API ASimpleWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleWeapon(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	UBoxComponent* WeaponCollision;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackAnim;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* Attack2Anim;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* SprintAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class ASimpleEffect> AttackEppectClass;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void SetOnwingPawn(APublicCharacter* NewOwner);

	void AttachMeshtoPawn();

	void StartAttack();
	void StopAttack();
	void ComboAttack();

	class APublicCharacter* MyPawn;

	bool bDuringAttack;
	bool bAttackComplite;
	bool bSaveAttack;
	int iCombo;
	
};
